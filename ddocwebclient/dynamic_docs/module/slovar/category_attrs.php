<?
$enable_sort_fields = array(
    'id'
);


$from = intval(get_request_variable('from', 0));
$id = intval(get_request_variable('id', 0));
$sort = get_request_variable('sort', 'date');


$sort_dir = get_request_variable('sort_dir', 'asc');

if (array_search($sort_dir, $convert_sort_dirs)===false) $sort_dir = 'asc';
$additional_vars = 'sort/'.urlencode($sort).'/sort_dir/'.urlencode($sort_dir);

$pattern = get_request_variable('pattern', '');



//load io_ids from rubricator
$id_io = 0;

if($id > 0)
{
	
	$rubricator = new rubricator();
	$rubricator->id = $id;
	$rubricator->Load();
	
	  $coms[] = array();
	
	  $agr_collection = new collection();
      $agr_collection->Load(TABLE_IO_RUBRICATOR, false, 'id_rubric='.$rubricator->id, 'id_rubric ASC');
          
      
        
             		for ($k=0;$k<count($agr_collection->_collection);$k++) {
             			
             			$name = '';
             			if($agr_collection->_collection[$k]->id_io_object > 0)
             			{
             				
             				$io = new io_objects();
             				$io->id = $agr_collection->_collection[$k]->id_io_object;
             				
             				if($io->Load())
             				{
             					$name = $io->name;
             				}
             			}
             			
                		$id_io = $agr_collection->_collection[$k]->id_io_object;
                		
                		
                		
                		
        if($pattern != '')
        {
        	$query = 'SELECT COUNT(*) AS num FROM iogetobjectattrsall('.$id_io.');';
        }
        else 
        {

			$query = 'SELECT COUNT(*) AS num  FROM iogetobjectattrsall('.$id_io.');';
        }

		$db->query($query);
		//echo $query;

		$all_num = $db->value[0]['num'];

		
		if ($all_num>0) {
    
		if($pattern != '')
        	{
			
        		$query = 'SELECT * FROM iogetobjectattrsall('.$id_io.') ORDER BY id_io_attribute;';
        	}
        	else 
        	{
        		$query = 'SELECT * FROM iogetobjectattrsall('.$id_io.') ORDER BY id_io_attribute;';
        	}
	
	
		$db->query($query);
		//echo "n - ".$k." ".$query;
    	$coms[$k] = $db->value;
    }
                		
                		
                		
                		
                		
                		
                		
            		}
	
	
	
	






}
$cats = new collection();
$cats->Load(TABLE_ATYPES, false, $where, 'name ASC');
$cats = $cats->_collection;


$names = new collection();
$names->Load(TABLE_ATTRIBUTES, false, $where, 'name ASC');
$names = $names->_collection;

$page_break = get_page_break(SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action'].'/', $all_num, $from, LIMIT_COMPANY_ON_PAGE, LIMIT_PAGE_ON_PAGE, $additional_vars);


include SITE_FILE_ROOT.'template/header.php';
include SITE_FILE_ROOT.'template/'.$STORAGE['module'].'/category_attrs.php';
include SITE_FILE_ROOT.'template/footer.php';
