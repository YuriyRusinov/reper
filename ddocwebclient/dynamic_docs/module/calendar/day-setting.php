<?
$page_title = 'Редактирование настроек дня';

$date = get_request_variable('date', date('Y-m-d'));
$start_work = get_request_variable('start_work', '');
$stop_work = get_request_variable('stop_work', '');
$status = get_request_variable('status', '');
$description = get_request_variable('description', '');
if ($status!='work') {
    $start_work = '00:00';
    $stop_work = '00:00';
}

$save = get_request_variable('save', '');
$_REQUEST['callback'] = get_request_variable('callback', '');

$employee_date = new employee_day();
$employee_date->employee_id = $STORAGE['current_employee']->id_employee;
$employee_date->date = $date;
$employee_date->Load();

if ($save) {
    if ($status!='') {
        $employee_date->start_work = $start_work.':00';
        $employee_date->stop_work = $stop_work.':00';
        $employee_date->status = $status;
        $employee_date->description = $description;

        $errors = $employee_date->IsValidData();
        if (count($errors)==0) {
            $db = db_class::get_instance();
            $db->begin();

            $employee_date->Save();

            $db->commit();
        }
    } else {
        $employee_date->Delete();
    }
} else {
    $start_work = format_time('{H}:{i}', $employee_date->start_work);
    $stop_work = format_time('{H}:{i}', $employee_date->stop_work);
}

include SITE_FILE_ROOT.'template/simple_header.php';
include SITE_FILE_ROOT.'template/'.$STORAGE['module'].'/'.$STORAGE['action'].'.php';
include SITE_FILE_ROOT.'template/simple_footer.php';
