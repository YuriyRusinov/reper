<?
$page_title = 'Установка прав доступа';

$destination = get_request_variable('destination', '');
$destination = preg_replace('/[\/]{2,}/', '/', $destination);
$enabled_file_roles = get_request_variable('enabled_file_roles', array());
$save = get_request_variable('save', '');
$_REQUEST['callback'] = get_request_variable('callback', '');

$destination_win1251 = mb_convert_encoding($destination, 'windows-1251', 'utf-8');
$destination_win1251 = SITE_PROJECT_FILE_ROOT.str_replace('..', '', $destination_win1251);

$file = new file();
if (!$file->LoadByCrc(crc($destination))) {
    if (file_exists($destination_win1251) && is_file($destination_win1251)) {
        $file->name = basename($destination);
        $file->path = dirname($destination);
        $file->Save();
    } else {
        $errors[] = 'Файл не найден';
    }
}

$errors = array();
if ($save) {
    if (!$enabled_file_roles) $errors[] = 'Укажите доступные роли';

    if (!$errors) {
        $db = db_class::get_instance();
        $db->begin();

        $file->DeleteFileRoles();

        reset($enabled_file_roles);
        $file_role = new file_role();
        $file_role->file_id = $file->id_file;
        while(list(,$role_id)=each($enabled_file_roles)) {
            $file_role->role_id = $role_id;
            $file_role->Save();
        }

        $db->commit();
    }
} else {
    $enabled_file_roles = $file->GetFileRoleIds();
}

$enabled_file_roles = array_flip($enabled_file_roles);

$roles = new collection();
$roles->Load(TABLE_ROLE, false, '', 'name ASC');
$roles = $roles->_collection;

include SITE_FILE_ROOT.'template/simple_header.php';
include SITE_FILE_ROOT.'template/'.$STORAGE['module'].'/'.$STORAGE['action'].'.php';
include SITE_FILE_ROOT.'template/simple_footer.php';
