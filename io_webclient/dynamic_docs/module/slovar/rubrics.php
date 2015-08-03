<?

$id = intval(get_request_variable('id', 0));


include SITE_FILE_ROOT.'template/header.php';

include SITE_FILE_ROOT.'template/'.$STORAGE['module'].'/rubrics.php';

include SITE_FILE_ROOT.'template/footer.php';
