<?
$page_title = 'Удаление события';

$errors = array();
$id = intval(get_request_variable('id', 0));
$_REQUEST['callback'] = get_request_variable('callback', '');
$date=get_request_variable('date', 0);

$event = new calendar_event();
$event->id_event = $id;
if ($id > 0) {
    $event->Delete();    
};

header('Location:'.SITE_HTTP_ROOT.$STORAGE['module'].'/'.$_REQUEST['callback'].'/date/'.$date.'/');

