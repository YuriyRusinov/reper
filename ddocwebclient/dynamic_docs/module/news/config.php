<?
$save=get_request_variable('save',0);

$news_conf = new news_config();

$news_conf->id = 1;
$news_conf->Load();

if($save){
    $am_news =      intval(get_request_variable('am_news', 10));
    $PreLengthC = 	intval(get_request_variable('PreLengthC',100));
    $tab_width = 	get_request_variable('tab_width','100%');
    $p_time = 		intval(get_request_variable('p_time',1));
    $p_date = 		intval(get_request_variable('p_date',1));
    $img_height = 	intval(get_request_variable('img_height',20));

    $news_conf->am_news = $am_news;
  	$news_conf->PreLengthC = $PreLengthC;
  	$news_conf->tab_width = $tab_width;
  	$news_conf->p_time = $p_time;
  	$news_conf->p_date = $p_date;
  	$news_conf->img_height = $img_height;
  	$errors = $news_conf->IsValidData();
	
    if (count($errors)==0) {
        $db->begin();
        
        $news_conf->Save();
        
        $db->commit();    
    };
    
}


include SITE_FILE_ROOT.'template/header.php';
include SITE_FILE_ROOT.'template/'.$STORAGE['module'].'/'.$STORAGE['action'].'.php';
include SITE_FILE_ROOT.'template/footer.php';


?>