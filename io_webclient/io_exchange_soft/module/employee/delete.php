<?
$page_title = 'Удаление данных о сотруднике';

$errors = array();
$id = intval(get_request_variable('id', 0));
$_REQUEST['callback'] = get_request_variable('callback', '');

$employee = new employee();
$employee->id_employee = $id;
if ($id > 0) {
    if ($employee->Load()) {
        $errors = $employee->IsValidData();
        if (!$errors) {
        	//set flag to not active
        	
          	$employee->delete_flag = 'yes';
        	
            $employee->Save();
        }
    }
}

include SITE_FILE_ROOT.'template/simple_header.php';
include SITE_FILE_ROOT.'template/'.$STORAGE['module'].'/'.$STORAGE['action'].'.php';
include SITE_FILE_ROOT.'template/simple_footer.php';
