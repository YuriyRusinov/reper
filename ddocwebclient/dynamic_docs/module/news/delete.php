<?
$id = get_request_variable('id', '');

$news = new news();
$news->news_id=$id;
if ($id) {
    if ($news->Load()) {
        $dir= SITE_NEWS_FILE_ROOT.$news->news_stamp.$news->news_exp;
        @unlink ($dir);
        $dir= SITE_NEWS_FILE_ROOT.$news->news_stamp."_1".$news->news_exp;
        @unlink ($dir);
        $date=$news->news_date;
        $news->Delete();
    }else{
        $news->news_id=0;  
        $date=date('Y-m-d');
    }    
};

header('Location:'.SITE_HTTP_ROOT.$STORAGE['module'].'/edit/date/'.$date.'/');       
?>