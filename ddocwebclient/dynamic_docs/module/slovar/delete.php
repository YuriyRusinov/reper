<?
$page_title = 'Удаление информационного объекта';

$errors = array();
$id = intval(get_request_variable('id', 0));
$_REQUEST['callback'] = get_request_variable('callback', '');

$io_object = new io_objects();
$io_object->id = $id;
if ($id > 0) {
    if ($io_object->Load()) {
        $errors = $io_object->IsValidData();
        if (!$errors) {
            $io_object->Delete();
        }
    }
}

include SITE_FILE_ROOT.'template/simple_header.php';
include SITE_FILE_ROOT.'template/'.$STORAGE['module'].'/'.$STORAGE['action'].'.php';
include SITE_FILE_ROOT.'template/simple_footer.php';
