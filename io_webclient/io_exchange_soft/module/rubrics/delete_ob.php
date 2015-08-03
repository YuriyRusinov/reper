<?
$page_title = 'Удаление объекта из рубрики';

$errors = array();
$id = intval(get_request_variable('id', 0));
$rubric_id = intval(get_request_variable('rubric_id', 0));

$_REQUEST['callback'] = get_request_variable('callback', '');

$io_rubricator = new io_rubricator();
$io_rubricator->id_io_object = $id;
$io_rubricator->id_rubric = $rubric_id;
if ($id > 0) {
            $io_rubricator->Delete();    
}

include SITE_FILE_ROOT.'template/simple_header.php';
include SITE_FILE_ROOT.'template/'.$STORAGE['module'].'/'.$STORAGE['action'].'.php';
include SITE_FILE_ROOT.'template/simple_footer.php';
