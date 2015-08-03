<?
$enable_sort_fields = array(
    'id'
);

//get table name
$id = intval(get_request_variable('id', 0));

$records = array();

$attr_types = array();

if($id > 0)
{
	$io_object = new io_objects();
	$io_object->id = $id;
	
	$table_name = '';

	if ($id > 0) {
    	if (!$io_object->Load()) {
        	$io_object->id = 0;
    	}
    	else 
    	{
    		$table_name =  $io_object->table_name;
    		
    		$parent_category = new io_categories();
    		$parent_category->id = $io_object->id_io_category;
    		
    		if(!$parent_category->Load())
    		{
    			$errors[] = "Can't get category!!!";
    		}
    		};
    		  		
	}
    	
	}
	

	if($parent_category->id_child > 0)
	{
		 $db = db_class::get_instance();

         $query = 'SELECT * FROM cgetcategoryattrs('.$parent_category->id_child.');';
		
       // echo $query;
         
		 $db->query($query);
		 $data = array();
		 $attr_title = array();
		 $attr_ids = array();
		 $attr_show = array();
		 
         $attr_types =  $db->value;
         
         $attr_val = array();
         
         for ($i=0; $i<count($attr_types); $i++) {

         	if($attr_types[$i]['id_attr_type'] != 17)
         	{
         		array_push ($data,$attr_types[$i]['attr_code']);
         		array_push ($attr_title,$attr_types[$i]['attr_title']);
         		array_push ($attr_ids,$attr_types[$i]['id_attr_type']);
         		array_push ($attr_show,$attr_types[$i]['is_read_only']);

         		$attr_val[] = $attr_types[$i];
         		
         	}
         
         	
         }
          
         $query = 'SELECT '; 
         
        $q_datas = $data;
         
         for($i=0;$i<count($q_datas);$i++)
         {
         	
         	if($attr_val[$i]['id_attr_type'] == 15)
         	{
         		 $tmp_v = $q_datas[$i];
         	     $tmp_v = 'rxpm2jpeg("'.$tmp_v.'"::bytea) as l'.$i;
         	     $data[$i] = l.$i;
         	     $q_datas[$i] = $tmp_v;
         	}
         	else {
         	   $tmp_v = $q_datas[$i];
         	   $tmp_v = '"'.$tmp_v.'"';
         	   $q_datas[$i] = $tmp_v;
         	}
         	
         }
         
         $query .= join(', ', $q_datas);
         
         $query .= ' FROM '.$table_name; 
         
         //echo $query;
         $db->query($query);
		 $records = $db->value;
	}




$page_break = get_page_break(SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action'].'/', $all_num, $from, LIMIT_COMPANY_ON_PAGE, LIMIT_PAGE_ON_PAGE, $additional_vars);


include SITE_FILE_ROOT.'template/header.php';
include SITE_FILE_ROOT.'template/'.$STORAGE['module'].'/tables.php';
include SITE_FILE_ROOT.'template/footer.php';
