<?
$page_title = 'Удаление должности';

$errors = array();
$id = intval(get_request_variable('id', 0));
$_REQUEST['callback'] = get_request_variable('callback', '');

$office = new office();
$office->id_office = $id;
if ($id > 0) {
    if ($office->Load()) {
        $errors = $office->IsValidData();
        if (!$errors) {
            $office->Delete();
        }
    }
}

include SITE_FILE_ROOT.'template/simple_header.php';
include SITE_FILE_ROOT.'template/'.$STORAGE['module'].'/'.$STORAGE['action'].'.php';
include SITE_FILE_ROOT.'template/simple_footer.php';
