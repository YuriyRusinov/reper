<?
$news = new news();
$news->news_date=date('Y-m-d');
$title = 'Новое объявление';
$STORAGE['action']='edit';
include SITE_FILE_ROOT.'template/header.php';
include SITE_FILE_ROOT.'template/'.$STORAGE['module'].'/'.$STORAGE['action'].'-id.php';
include SITE_FILE_ROOT.'template/footer.php';
?>