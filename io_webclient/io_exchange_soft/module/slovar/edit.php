<?
$id = intval(get_request_variable('id', 0));

$table_name = get_request_variable('table_name', '');
$name = get_request_variable('name', '');
$information = get_request_variable('information', '');

$cat_id = intval(get_request_variable('cat_id', 0));

$jr_id = intval(get_request_variable('jr_id', 0));

$mac_id = intval(get_request_variable('mac_id', 0));
$date = get_request_variable('date','');
$aut_id = intval(get_request_variable('aut_id', 0));

$is_system = get_request_variable('is_system',false);


$date = parse_date('{Y}-{m}-{d}',$date,false);
$description = get_request_variable('description','');
$state_id = intval(get_request_variable('state_id', 0));


$save = get_request_variable('save', '');


$_REQUEST['callback'] = get_request_variable('callback', '');

$db = db_class::get_instance();


$io_object = new io_objects();
$io_object->id = $id;


if($cat_id > 0 )
{
	$io_object->id_io_category = $cat_id;
}

if ($id > 0) {
    if (!$io_object->Load()) {
        $io_object->id = 0;
    }
}


if ($save) {       			
     $io_object->id_io_category 		= $cat_id;
     $io_object->id_io_state	 	= $state_id;
     $io_object->name 				= $name; 

     if($id > 0 )
     {
     	//echo "author ".$io_object->id_maclabel."<br>";
     }
     else 
     {
     	$io_object->author				= $aut_id;
     	$io_object->id_maclabel		= $mac_id;
     
     }
     
     $io_object->description        = $description;
    
	 $io_object->information		= $information;
     $io_object->is_system 			= $is_system;
     $io_object->insert_time 		= $date;
     
     
   
     if($id == 0)
     {  
    	
		$io_object->is_system = 'false';
		$io_object->id_io_state = 1;
		$io_object->table_name = NULL;
		
     }
	
	
	
    $errors = $io_object->IsValidData();
  
   
    if (count($errors)==0) {
       
        $db->begin();
        $io_object->Save();
        $db->commit();
        
        
        $id = $io_object->id;
        
        //io_object must be assign with journal
        if($jr_id > 0)
        {
        	
        	$query = "SELECT cjInsert(".$id.",".$jr_id.",TRUE);";
        	
        	//echo $query;
        	
        	$db->query($query);
        	
        }
        
        
    }
    else 
    {
        $save=0;
    }
}


for($i=0;$i<count($STORAGE['menu']);$i++){
    if(strpos($STORAGE['menu'][$i]['path'],'slovar')!==false){
        $menulevel=$STORAGE['menu'][$i];
        break;        
    }
}
$STORAGE['site_path'][]=array('name'=>$menulevel['name'],'path'=>$menulevel['path']);
$menulevel=$menulevel['childs'][0];
$STORAGE['site_path'][]=array('name'=>$menulevel['name'],'path'=>$menulevel['path']);

if ($save)
{ 
	?>    
<script language="JavaScript">
    
<!--
    
window.location =  "<?=SITE_HTTP_ROOT.$STORAGE['site_path'][count($STORAGE['site_path'])-2]['path']?>";

//-->
</script>
<?
}
else 
{
	$empl = new collection();
	$empl->Load(TABLE_USERS, false, $where, 'fio ASC');
	$empl = $empl->_collection;
		   
	$states = new collection();
	$states->Load(TABLE_IO_STATES, false, $where, 'name ASC');
	$states = $states->_collection;
	
	$cats = new collection();
	$cats->Load(TABLE_CATEGORIES, false, $where, 'name ASC');
	$cats = $cats->_collection;
	
	
    $menulevel=$menulevel['childs'][0];
    $STORAGE['site_path'][]=array('name'=>$menulevel['name'],'path'=>$menulevel['path']);
    $path_name='Редактировать';
  

    $STORAGE['site_path'][]=array('name'=>$path_name,'path'=>$STORAGE['request_uri']);
    $page_title = $path_name;

    include SITE_FILE_ROOT.'template/header.php';
    include SITE_FILE_ROOT.'template/'.$STORAGE['module'].'/'.$STORAGE['action'].'.php';
    include SITE_FILE_ROOT.'template/footer.php';    
};