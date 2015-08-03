<?
$page_title = 'Загрузка файлов';

$name = get_request_variable('name', '');
$destination = get_request_variable('destination', '');
$enabled_file_roles = get_request_variable('enabled_file_roles', array());
$save = get_request_variable('save', '');
$_REQUEST['callback'] = get_request_variable('callback', '');

$destination_win1251 = mb_convert_encoding($destination, 'windows-1251', 'utf-8');
$destination_win1251 = str_replace('..', '', $destination_win1251);

$name_win1251 = mb_convert_encoding($name, 'windows-1251', 'utf-8');
$name_win1251 = str_replace('..', '', $name_win1251);

$file = new file();

$errors = array();
if ($save) {
    $file->name = trim($name);
    $file->path = $destination;
    $errors = $file->IsValidData();

    if ($name_win1251!='') {
        $file_destination = SITE_PROJECT_FILE_ROOT.($destination_win1251.'/'.$name_win1251);
        if (file_exists($file_destination)) $errors[] = 'Файл с таким именем уже существует';
    }

    $uploaded_file = '';
    if ($_FILES['file']['error']==UPLOAD_ERR_OK) {
        $uploaded_file = $_FILES['file']['tmp_name'];
    } else {
        if ($_FILES['file']['error']!=UPLOAD_ERR_NO_FILE) {
            $errors[] = 'Ошибка загрузки файла: ERROR '.$_FILES['file']['error'];
        } else {
            $errors[] = 'Укажите загружаемый файл';
        }
    }

    if (!$enabled_file_roles) $errors[] = 'Укажите доступные роли';

    if (!$errors) {
        copy($uploaded_file, $file_destination);

        $db = db_class::get_instance();
        $db->begin();

        $file->Save();

        reset($enabled_file_roles);
        $file_role = new file_role();
        $file_role->file_id = $file->id_file;
        while(list(,$role_id)=each($enabled_file_roles)) {
            $file_role->role_id = $role_id;
            $file_role->Save();
        }

        $db->commit();
    }
}

$enabled_file_roles = array_flip($enabled_file_roles);

$roles = new collection();
$roles->Load(TABLE_ROLE, false, '', 'name ASC');
$roles = $roles->_collection;

include SITE_FILE_ROOT.'template/simple_header.php';
include SITE_FILE_ROOT.'template/'.$STORAGE['module'].'/'.$STORAGE['action'].'.php';
include SITE_FILE_ROOT.'template/simple_footer.php';
