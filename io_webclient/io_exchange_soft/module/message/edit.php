<?
$id = intval(get_request_variable('id', 0));

$date = get_request_variable('date','');
$date2 = get_request_variable('date2', '');
$start_time = get_request_variable('start_time', '');
$stop_time = get_request_variable('stop_time', '');

$sender_id = intval(get_request_variable('sender_id', 0));
$receiver_id = intval(get_request_variable('receiver_id', 0));
$io_id = intval(get_request_variable('io_id', 0));

$date = parse_date('{Y}-{m}-{d}',$date,false);
$date2 = parse_date('{Y}-{m}-{d}',$date2,false);

$description = get_request_variable('description','');

$save = get_request_variable('save', '');

$_REQUEST['callback'] = get_request_variable('callback', '');

$db = db_class::get_instance();

$message_journal = new message_journal();
$message_journal->id = $id;

if ($id > 0) {
    if (!$message_journal->Load()) {
        $message_journal->id = 0;
    }
}
else {
	
	$sender_id = $_SESSION['PST'];
}

if ($save) {
           
    $message_journal->id_dl_sender 		 = 	$sender_id ;//ика
    $message_journal->id_dl_receiver 	 = 	$receiver_id;//ответственный
    $message_journal->id_io_object 		 = 	$io_id;//исполнитель
   
    $message_journal->insert_time = $date." ".$start_time;
    $message_journal->sent_datetime = $date2." ".$stop_time;
    
    $message_journal->receive_datetime = NULL;                            
    $message_journal->read_datetime = NULL;
    
    $message_journal->message_body = $description;
     
    
     if($id == 0)
     {  
    	$message_journal->is_outed = 'true';                	  
     }
	
	
	
    $errors = $message_journal->IsValidData();
  
   
    if (count($errors)==0) {
       
        $db->begin();
        $message_journal->Save();
        $db->commit();
    }
    else 
    {
        $save=0;
    }
}


for($i=0;$i<count($STORAGE['menu']);$i++){
    if(strpos($STORAGE['menu'][$i]['path'],'message')!==false){
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
	$empl = new collection();
	$empl->Load(TABLE_DLS, false, $where, 'name ASC');
	$empl = $empl->_collection;

	$objects = new collection();
	$objects->Load(TABLE_OBJECTS, false, $where, 'name ASC');
	$objects = $objects->_collection;
	
        $menulevel=$menulevel['childs'][0];
        $STORAGE['site_path'][]=array('name'=>$menulevel['name'],'path'=>$menulevel['path']);
        $path_name='Редактировать';
  

    $STORAGE['site_path'][]=array('name'=>$path_name,'path'=>$STORAGE['request_uri']);
    $page_title = $path_name;

    include SITE_FILE_ROOT.'template/header.php';
    include SITE_FILE_ROOT.'template/'.$STORAGE['module'].'/'.$STORAGE['action'].'.php';
    include SITE_FILE_ROOT.'template/footer.php';    
};