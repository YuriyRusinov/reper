<?
if (!defined("COLLECTION")) {
    define ("COLLECTION",true);

class collection
{
    var $_collection;

    // CONSTRUCTOR
    function collection()
    {
        $this->_Init();
    }

    function Load($table, $count_found_record=false, $where='', $order='', $from='', $limit='', $select='')
    {
        $db = db_class::get_instance();

        $query = 'SELECT * FROM '.$table;
        if ($where) $query.=' WHERE '.$where;
        if ($count_found_record) $query_count = str_replace('SELECT * FROM', 'SELECT COUNT(*) AS num FROM',$query);
        if ($select) $query = str_replace('SELECT * FROM', 'SELECT '.$select.' FROM',$query);
        if ($order) $query.=' ORDER BY '.$order;
        //if (!is_string($from)) $query.=' LIMIT '.$from;
        if ($limit) $query.=' LIMIT '.$limit;//, - deleted
        
        //echo $query;
        
        $db->query($query);

        $this->LoadItems($table, $db->value);

        if ($count_found_record) {
            $db->query($query_count);
            return (int)$db->value[0]['num'];
        }

        return true;
    }

    function LoadItems($class, &$db_records)
    {
        $db = db_class::get_instance();

        for ($i=0;$i<count($db_records);$i++) {
            $collection_item = new $class();

            $collection_item->LoadItem($db_records[$i]);
            $this->_collection[]=$collection_item;
        }
    }

    function Save(&$db)
    {
        @reset($this->_collection);
        while(list(, $collection_item)=@each($this->_collection)) {
            $collection_item->Save();
        }
    }

    function Delete(&$db)
    {
        while(list(, $collection_item)=@each($this->_collection)) {
            $collection_item->Delete();
        }
    }

    // PRIVATE FUNCTIONS
    function _Init()
    {
        $this->_collection = array();
    }
}

}