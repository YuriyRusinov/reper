<?
header("Content-type:text/xml");

$id = get_request_variable('id', 'root');
if ($id!='root') $id = intval($id);

$db = db_class::get_instance();

echo '<?xml version="1.0" ?>'."\n";
if ($id==='root') {
    $department = new department();
    $department->id_department = null;
    $childs = $department->ChildExists();
    echo '<tree id="root">'."\n";
    echo '<item text="ККС СИТО" id="0" child="'.($childs ? 1:0).'"></item>';
    echo '</tree>'."\n";
} else {
    $type_collection = new collection();
    $type_collection->Load(TABLE_DEPARMENT, false, ($id > 0 ? 'parent_id='.$id : 'parent_id IS NULL'), 'name ASC');
    echo '<tree id="'.$id.'">'."\n";
    for ($i=0;$i<count($type_collection->_collection);$i++) {
        $childs = $type_collection->_collection[$i]->ChildExists();
        echo '<item child="'.($childs ? 1:0).'" id="'.$type_collection->_collection[$i]->id_department.'" text="'.escape($type_collection->_collection[$i]->name).'" im0="folderOpen.gif" im1="folderOpen.gif" im2="folderClosed.gif">';
        echo '</item>';
    }
    echo '</tree>'."\n";
}
