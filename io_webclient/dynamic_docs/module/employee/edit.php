<?
$page_title = 'Редактирование данных сотрудника';

$id = intval(get_request_variable('id', 0));
$name = get_request_variable('name', '');
$active_flag = get_request_variable('active_flag', 'no');
$delete_flag = get_request_variable('delete_flag', 'no');
$login = get_request_variable('login', '');
$password = get_request_variable('password', '');
$email = get_request_variable('email', '');
$department_id = intval(get_request_variable('department_id', 0));
$head_id = intval(get_request_variable('head_id', 0));
$enabled_employee_roles = get_request_variable('enabled_employee_roles', array());
$post = get_request_variable('post', '');
$birthday = get_request_variable('birthday', '');
$home_address = get_request_variable('home_address', '');
$office_phone = get_request_variable('office_phone', '');
$home_phone = get_request_variable('home_phone', '');
$cellular = get_request_variable('cellular', '');
$room = get_request_variable('room', '');
$save = get_request_variable('save', '');
$_REQUEST['callback'] = get_request_variable('callback', '');

$employee = new employee();
$employee->id_employee = $id;
if ($id > 0) {
    if (!$employee->Load()) {
        $employee->id_employee = 0;
    }
}

if ($save) {
    $employee->department_id = $department_id;
    $employee->head_id = $head_id;
    if ($employee->department_id === 0) $employee->department_id = null;
    if ($employee->head_id === 0) $employee->head_id = null;

    $employee->name = $name;
    $employee->active_flag = $active_flag;
    $employee->delete_flag = $delete_flag;
    $employee->login = $login;
    $employee->password = $password;
    $employee->email = $email;
    $employee->post = $post;
    $employee->birthday = parse_date('{Y}-{m}-{d}', $birthday, false);
    $employee->home_address = $home_address;
    $employee->office_phone = $office_phone;
    $employee->home_phone = $home_phone;
    $employee->cellular = $cellular;
    $employee->room = $room;
    
    $errors = $employee->IsValidData();

    $uploaded_file = '';
    if ($_FILES['photo']['error']==UPLOAD_ERR_OK) {
        $uploaded_file = $_FILES['photo']['tmp_name'];
        if (!$employee->CheckImageType($uploaded_file)) {
            $errors[] = 'Неверный формат изображения. Допустимые форматы: gif/jpeg/png';
        }
    } else {
        if ($_FILES['photo']['error']!=UPLOAD_ERR_NO_FILE) {
            $errors[] = 'Ошибка загрузки фотографии: ERROR '.$_FILES['photo']['error'];
        }
    }

    if (count($errors)==0) {
        $employee->birthday = parse_date('{Y}-{m}-{d}', $birthday, true);

        $employee->Save();

        if ($uploaded_file!='') {
            $employee->UploadPhoto($uploaded_file);
            $employee->SetPhotoFlag(true);
        }

        $db = db_class::get_instance();
        $db->begin();

        // Save user roles
        if(access_enable($STORAGE,$STORAGE['module'],'role')){
            $employee->DeleteEmployeeRoles();
    
            reset($enabled_employee_roles);
            $employee_role = new employee_role();
            $employee_role->employee_id = $employee->id_employee;
            while(list(,$role_id)=each($enabled_employee_roles)) {
                $employee_role->role_id = $role_id;
                $employee_role->Save();
            }
        };  

        $db->commit();
    }
} else {
    $enabled_employee_roles = $employee->GetEmployeeRolesId();
}

$department_name = '---';
if ($employee->department_id>0) {
    $department = new department();
    $department->id_department = $employee->department_id;
    if ($department->Load()) $department_name = $department->name;
}

$enabled_employee_roles = array_flip($enabled_employee_roles);

$where = '';
//if ($employee->id_employee>0) $where = 'id_employee<>'.$employee->id_employee;
$employees = new collection();
$employees->Load(TABLE_EMPLOYEE, false, $where, 'name ASC');
$employees = $employees->_collection;

$where = '';
if(!check_current_employee_admin($STORAGE)) $where='"super_user"=\'no\'';
$roles = new collection();
$roles->Load(TABLE_ROLE, false, $where, 'name ASC');
$roles = $roles->_collection;

$offices = new collection();
$offices->Load(TABLE_OFFICE,false,'','name ASC');
$offices = $offices->_collection;



include SITE_FILE_ROOT.'template/simple_header.php';
include SITE_FILE_ROOT.'template/'.$STORAGE['module'].'/'.$STORAGE['action'].'.php';
include SITE_FILE_ROOT.'template/simple_footer.php';
