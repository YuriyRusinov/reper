<?
$page_title = 'Удаление рубрики';

$errors = array();
$id = intval(get_request_variable('id', 0));
$_REQUEST['callback'] = get_request_variable('callback', '');

$rubricator = new rubricator();
$rubricator->id = $id;
if ($id > 0) {
    if ($rubricator->Load()) {
        $errors = $rubricator->IsValidData();
        if (!$errors) {
            $rubricator->Delete();
        }
    }
}

include SITE_FILE_ROOT.'template/simple_header.php';
include SITE_FILE_ROOT.'template/'.$STORAGE['module'].'/'.$STORAGE['action'].'.php';
include SITE_FILE_ROOT.'template/simple_footer.php';
