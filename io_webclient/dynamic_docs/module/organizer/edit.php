<?
$id = get_request_variable('id', '');
$save = get_request_variable('save', '');
$start_date=get_request_variable('start_date',date('Y-m-d'));
$stop_date=get_request_variable('stop_date',date('Y-m-d'));

$calendar = new calendar();
$calendar->loadDefault($STORAGE['current_employee']->login);

$ids=calendar::getIdsForAuthor($STORAGE['current_employee']->login);

$event = new calendar_event();
$event->id_event=$id;
if ($id) {
    if (!$event->Load()) {
        $event->id_event = 0;
    }elseif(!in_array($event->calendar_id,$ids)){
        $event->init();
    }
}else{
    $event->id_event=0;
}

if($save){
    
    if($event->calendar_id==0){
        $event->calendar_id=$calendar->id_calendar;
    }
    $event->what = get_request_variable('what','');
    $event->where = get_request_variable('where','');
    $event->description = get_request_variable('description','');
    $event->completed = get_request_variable('completed','no');
    $event->notice = get_request_variable('notice','no');
    $event->mark = get_request_variable('mark','no');
    $event->start_date = parse_date('{Y}-{m}-{d}',$start_date,false);
    if(!trim($stop_date)) $stop_date=$start_date;
    $event->stop_date = parse_date('{Y}-{m}-{d}',$stop_date,false);
    $wholeday=get_request_variable('wholeday','');
    if($wholeday){
       $event->start_time = null;
       $event->stop_time = null;
    }else{
       $start_time = get_request_variable('start_time',date('H:i'));
       $event->start_time = parse_time('{H}:{i}',$start_time);
       $stop_time = get_request_variable('stop_time',date('H:i'));
       $event->stop_time = parse_time('{H}:{i}',$stop_time);
    };

    $errors = $event->IsValidData();
    if (count($errors)==0) {
        $event->start_date = parse_date('{Y}-{m}-{d}',$start_date,true);
        $event->stop_date = parse_date('{Y}-{m}-{d}',$stop_date,true);
        $db->begin();

        $event->Save();

        $db->commit();        
    }
}


if($event->id_event == 0){    
   $event->start_date = $start_date;   
   $event->stop_date = $stop_date;
}

$wholeday=($event->start_time=='');
include SITE_FILE_ROOT.'template/header.php';
include SITE_FILE_ROOT.'template/'.$STORAGE['module'].'/'.$STORAGE['action'].'.php';
include SITE_FILE_ROOT.'template/footer.php';    


?>