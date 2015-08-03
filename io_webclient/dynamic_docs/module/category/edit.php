<?

$id = intval(get_request_variable('id', 0));
$parent_id = intval(get_request_variable('parent_id', 0));

$kio_id = get_request_variable('kio_id', 0);
$id_child = get_request_variable('id_child', 0);
$is_main = get_request_variable('is_main', 0);
$is_system = get_request_variable('is_system','FALSE');

$name = get_request_variable('name','');
$code = get_request_variable('code', '');

$description = get_request_variable('description','');

$save = get_request_variable('save', '');


$_REQUEST['callback'] = get_request_variable('callback', '');

$db = db_class::get_instance();


$category = new io_categories();
$category->id = $id;

if ($id > 0) {
    if (!$category->Load()) {
        $category->id = 0;
    }
}


if ($save && $kio_id > 0) {
           
    $category->is_system 	 		= 	$is_system ;//
    $category->is_main 			    = 	$is_main;//
    $category->id_child 		 	= 	$id_child;//
    $category->id_io_type 			= 	$kio_id;
           
    $category->name = $name;
    $category->code = $code;
    
    $category->description = $description;
     
        
     if($id == 0)
     {  
           
     }
		
   $errors = $category->IsValidData();
  
   
    if (count($errors)==0) {
       
        $db->begin();
        $category->Save();
        $db->commit();
    }
    else 
    {
        $save=0;
    }
}


for($i=0;$i<count($STORAGE['menu']);$i++){
    if(strpos($STORAGE['menu'][$i]['path'],'category')!==false){
        $menulevel=$STORAGE['menu'][$i];
        break;        
    }
}
$STORAGE['site_path'][]=array('name'=>$menulevel['name'],'path'=>$menulevel['path']);
$menulevel=$menulevel['childs'][0];
$STORAGE['site_path'][]=array('name'=>$menulevel['name'],'path'=>$menulevel['path']);

if ($save)
{     
        $menulevel=$menulevel['childs'][0];
        $STORAGE['site_path'][]=array('name'=>$menulevel['name'],'path'=>$menulevel['path']);
     
    
    header('Location:'.SITE_HTTP_ROOT.$STORAGE['site_path'][count($STORAGE['site_path'])-1]['path']);
}
else 
{		   	
	$cats = new collection();
	$cats->Load(TABLE_CATEGORIES, false, $where, 'name ASC');
	$cats = $cats->_collection;
	
	
	$childs = new collection();
	$childs->Load(TABLE_CATEGORIES, false, "is_main = FALSE", 'name ASC');
	$childs = $childs->_collection;
	
	
        $menulevel=$menulevel['childs'][0];
        $STORAGE['site_path'][]=array('name'=>$menulevel['name'],'path'=>$menulevel['path']);
        $path_name='Редактировать';
  

    $STORAGE['site_path'][]=array('name'=>$path_name,'path'=>$STORAGE['request_uri']);
    $page_title = $path_name;

    include SITE_FILE_ROOT.'template/header.php';
    include SITE_FILE_ROOT.'template/'.$STORAGE['module'].'/'.$STORAGE['action'].'.php';
    include SITE_FILE_ROOT.'template/footer.php';    
};