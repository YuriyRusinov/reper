<?
$date = get_request_variable('date', date('Y-m-d'));

$stop_day=strtotime('this Sunday',strtotime($date));
$stop_date=date('Y-m-d',$stop_day);
$start_day=strtotime('last Monday',$stop_day);
$start_date=date('Y-m-d',$start_day);

$calendar = new calendar();
$calendar->loadDefault($STORAGE['current_employee']->login);

$ids=calendar::getIdsForEmployee($STORAGE['current_employee']->login);
$ids=join(',',$ids);

$where=array();
$where[]='"calendar_id" in ('.$ids.')';
$where[]='"start_date"<=\''.$stop_date.'\' AND "stop_date">=\''.$start_date.'\'';
$where[]='"start_date" <> "stop_date"';

$query = 'SELECT * FROM '.TABLE_CALENDAR_EVENT.' WHERE '.join(' AND ', $where).' ORDER BY start_date,start_time,id_event ';
$db->query($query);
$week_events = $db->value;

$where=array();
$where[]='"calendar_id" in ('.$ids.')';
$where[]='"start_date"="stop_date"';
$where[]='"start_date"<=\''.$stop_date.'\' AND "stop_date">=\''.$start_date.'\'';

$query = 'SELECT * FROM '.TABLE_CALENDAR_EVENT.' WHERE '.join(' AND ', $where).' ORDER BY start_date,start_time,id_event ';
$db->query($query);
$time_events = $db->value;

include SITE_FILE_ROOT.'template/header.php';
include SITE_FILE_ROOT.'template/'.$STORAGE['module'].'/'.$STORAGE['action'].'.php';
include SITE_FILE_ROOT.'template/footer.php';    
