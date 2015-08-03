<?
$page_title = 'Просмотр данных сотрудника';

$id = intval(get_request_variable('id', 0));

$employee = new employee();
$employee->id_employee = $id;
if ($id > 0) {
    if (!$employee->Load()) {
        $employee->id_employee = 0;
    }
}

$department_name = '---';
if ($employee->department_id>0) {
    $department = new department();
    $department->id_department = $employee->department_id;
    if ($department->Load()) $department_name = $department->name;
}


$head = new employee();
if ($employee->head_id>0) {
    $head->id_employee = $employee->head_id;
    $head->Load();
}

include SITE_FILE_ROOT.'template/simple_header.php';
include SITE_FILE_ROOT.'template/'.$STORAGE['module'].'/'.$STORAGE['action'].'.php';
include SITE_FILE_ROOT.'template/simple_footer.php';
