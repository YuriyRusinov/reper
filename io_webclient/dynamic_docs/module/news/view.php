<?

$date = get_request_variable('date', '');
$id = get_request_variable('id', 0);

$news_conf = new news_config();


$news_conf->id = 1;
$news_conf->Load();

$am_news = $news_conf->am_news; //количество новостей
$PreLengthC = $news_conf->PreLengthC; //размер предпросмотра - число символов.
$tab_width = $news_conf->tab_width; //ширина таблицы с новостями
$p_time = $news_conf->p_time; //показывать время новости
$p_date = $news_conf->p_date; //показывать дату новости
$img_height = $news_conf->img_height; //высота для миникартинки новости

define('LIMIT_NEWS_ON_PAGE',$am_news);


if($id>0){
	
    $news = new news();
    $news->news_id=$id;
    $news->Load();
    include SITE_FILE_ROOT.'template/header.php';
    include SITE_FILE_ROOT.'template/'.$STORAGE['module'].'/'.$STORAGE['action'].'-id.php';
    include SITE_FILE_ROOT.'template/footer.php';

}else{

    if($date){
        $linedate=parse_date('{Y}-{m}-{d}', $date, true);
    }else{
        $linedate=date('Y-m-d',time());
    }
    
    $newses = array();
    
    $query = 'SELECT COUNT(*) as num FROM '.TABLE_NEWS.' WHERE news_date = \''.$linedate.'\'  limit '.LIMIT_NEWS_ON_PAGE;
    $db->query($query);
        
    $all_num = $db->value[0]['num'];
    
    if($all_num < LIMIT_NEWS_ON_PAGE)
    {

        $query = 'SELECT * FROM '.TABLE_NEWS.' WHERE news_date <= \''.$linedate.'\' ORDER BY news_id ASC  limit '.LIMIT_NEWS_ON_PAGE;
    	$db->query($query);
    	$newses = $db->value;
    	
    }
    else 
    {
    	$query = 'SELECT * FROM '.TABLE_NEWS.' WHERE news_date = \''.$linedate.'\' ORDER BY news_id ASC  limit '.LIMIT_NEWS_ON_PAGE;
    	$db->query($query);
        $newses = $db->value;
    }
    
    include SITE_FILE_ROOT.'template/header.php';
    include SITE_FILE_ROOT.'template/'.$STORAGE['module'].'/'.$STORAGE['action'].'-list.php';
    include SITE_FILE_ROOT.'template/footer.php';
    
}