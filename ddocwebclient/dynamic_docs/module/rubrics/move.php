<?
$page_title = 'Изменение рубрики';

$id = intval(get_request_variable('id', 0));
$parent_id = intval(get_request_variable('parent_id', 0));
$_REQUEST['callback'] = get_request_variable('callback', '');

$rubricator = new rubricator();
$rubricator->id = $id;
if ($id > 0) {
    if ($rubricator->Load()) {
        if ($parent_id===0) $parent_id = null;
        $rubricator->id_parent = $parent_id;
        $errors = $department->IsValidData();
        if (!$errors) {
            $rubricator->Save();
        }
    }
}

include SITE_FILE_ROOT.'template/simple_header.php';
include SITE_FILE_ROOT.'template/'.$STORAGE['module'].'/'.$STORAGE['action'].'.php';
include SITE_FILE_ROOT.'template/simple_footer.php';
