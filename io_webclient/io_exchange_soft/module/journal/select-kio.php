<?
define('DISABLE_EXPAND_WINDOW', true);
include SITE_FILE_ROOT.'template/simple_header.php';

include SITE_FILE_ROOT.'template/'.$STORAGE['module'].'/'.$STORAGE['action'].'.php';

include SITE_FILE_ROOT.'template/simple_footer.php';
