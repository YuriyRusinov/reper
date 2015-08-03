<?
$id = intval(get_request_variable('id', 0));

$kio = get_request_variable('kio', '');
$kio_id = get_request_variable('kio_id', 0);
$ika_id = get_request_variable('ika_id', 0);
$exec_id = get_request_variable('exec_id', 0);
$from_id = get_request_variable('from_id',0);


$jr_work = get_request_variable('jr_work',2);
$jr_state = get_request_variable('jr_state', '');

$date = get_request_variable('date','');
$date2 = get_request_variable('date2', '');
$start_time = get_request_variable('start_time', '');
$stop_time = get_request_variable('stop_time', '');


$date = parse_date('{Y}-{m}-{d}',$date,false);
$date2 = parse_date('{Y}-{m}-{d}',$date2,false);

$description = get_request_variable('description','');

if($id == 0)
{
	$jr_state = 1;
}

$save = get_request_variable('save', '');


$_REQUEST['callback'] = get_request_variable('callback', '');

$db = db_class::get_instance();


$journal = new journal();
$journal->id = $id;

if ($id > 0) {
    if (!$journal->Load()) {
        $journal->id = 0;
    }
}


for($i=0;$i<count($STORAGE['menu']);$i++){
    if(strpos($STORAGE['menu'][$i]['path'],'journal')!==false){
        $menulevel=$STORAGE['menu'][$i];
        break;        
    }
}
$STORAGE['site_path'][]=array('name'=>$menulevel['name'],'path'=>$menulevel['path']);
$menulevel=$menulevel['childs'][0];
$STORAGE['site_path'][]=array('name'=>$menulevel['name'],'path'=>$menulevel['path']);

if ($save)
{     
	
	if($jr_work == 1)
	{
		
		if ($id > 0) {
    	if (!$journal->Load()) {
        	$journal->id = 0;
    	}
		else 
		{ 
			//принимаем к исполнению
	
			$journal->id_jr_state = 3;
			$journal->Save();
			
			//ссылка на редактирование ИО                  			
	}
			}	
				
	}
		
	
        $menulevel=$menulevel['childs'][0];
        $STORAGE['site_path'][]=array('name'=>$menulevel['name'],'path'=>$menulevel['path']);
     
    if($journal->id_jr_state == 3)
    {
        header('Location:'.SITE_HTTP_ROOT.'slovar/edit/id/0/cat_id/'.$kio_id.'/jr_id/'.$journal->id.'/');
    }
    else 
    {
    	header('Location:'.SITE_HTTP_ROOT.$STORAGE['site_path'][count($STORAGE['site_path'])-1]['path']);
    }
}
else 
{
	$empl = new collection();
	$empl->Load(TABLE_DLS, false, $where, 'name ASC');
	$empl = $empl->_collection;

	$jrstat = new collection();
	$jrstat->Load(TABLE_JRSTATE, false, $where, 'name ASC');
	$jrstat = $jrstat->_collection;
		   
	
	$cats = new collection();
	$cats->Load(TABLE_CATEGORIES, false, $where, 'name ASC');
	$cats = $cats->_collection;
	
	
        $menulevel=$menulevel['childs'][0];
        $STORAGE['site_path'][]=array('name'=>$menulevel['name'],'path'=>$menulevel['path']);
        $path_name='Просмотр';
  

    $STORAGE['site_path'][]=array('name'=>$path_name,'path'=>$STORAGE['request_uri']);
    $page_title = $path_name;

    include SITE_FILE_ROOT.'template/header.php';
    include SITE_FILE_ROOT.'template/'.$STORAGE['module'].'/'.$STORAGE['action'].'.php';
    include SITE_FILE_ROOT.'template/footer.php';    
};