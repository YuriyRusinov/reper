<?
$page_title = 'Удаление категории';

$errors = array();
$id = intval(get_request_variable('id', 0));
$_REQUEST['callback'] = get_request_variable('callback', '');

$cat = new io_categories();
$cat->id = $id;
if ($id > 0) {
    if ($cat->Load()) {
        $errors = $cat->IsValidData();
        if (!$errors) {
            $cat->Delete();
        }
    }
}

include SITE_FILE_ROOT.'template/simple_header.php';
include SITE_FILE_ROOT.'template/'.$STORAGE['module'].'/'.$STORAGE['action'].'.php';
include SITE_FILE_ROOT.'template/simple_footer.php';
