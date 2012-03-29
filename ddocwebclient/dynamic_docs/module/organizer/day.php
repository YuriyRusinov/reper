<?

$date = get_request_variable('date', date('Y-m-d'));

if($date = "0000-00-00")
$date = date('Y-m-d');


$calendar = new calendar();
$calendar->loadDefault($STORAGE['current_employee']->login);

$ids=calendar::getIdsForEmployee($STORAGE['current_employee']->login);

$ids=join(',',$ids);
if ($ids == '') $ids = "NULL"; 

$where=array();
$where[]='"calendar_id" in ('.$ids.')';
$where[]='"start_date"<=\''.$date.'\' AND "stop_date">=\''.$date.'\'';
$where[]='"start_date" <> "stop_date"';

$query = 'SELECT * FROM '.TABLE_CALENDAR_EVENT.' WHERE '.join(' AND ', $where).' ORDER BY start_date,start_time,id_event ';
//echo $query;
$db->query($query);
$day_events = $db->value;

$where=array();
$where[]='"calendar_id" in ('.$ids.')';
$where[]='"start_date"=\''.$date.'\' AND "stop_date"=\''.$date.'\'';
$where[]='"start_time" IS NULL';

$query = 'SELECT * FROM '.TABLE_CALENDAR_EVENT.' WHERE '.join(' AND ', $where).' ORDER BY start_date,id_event ';
$db->query($query);
$day_events = array_merge($day_events,$db->value);

$where=array();
$where[]='"calendar_id" in ('.$ids.')';
$where[]='"start_date"=\''.$date.'\' AND "stop_date"=\''.$date.'\'';
$where[]='"start_time" IS NOT NULL';

$query = 'SELECT * FROM '.TABLE_CALENDAR_EVENT.' WHERE '.join(' AND ', $where).' ORDER BY "start_date","start_time","id_event" ';
$db->query($query);
$time_events = $db->value;

include SITE_FILE_ROOT.'template/header.php';
include SITE_FILE_ROOT.'template/'.$STORAGE['module'].'/'.$STORAGE['action'].'.php';
include SITE_FILE_ROOT.'template/footer.php';    
