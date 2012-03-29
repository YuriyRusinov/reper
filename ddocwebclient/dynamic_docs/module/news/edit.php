<?
$id = get_request_variable('id', '');
$date = get_request_variable('date', '');
$save = get_request_variable('save', '');

$news = new news();
$news->news_id=$id;
if ($id) {
    if (!$news->Load()) {
        $news->news_id = 0;
    }
}else{
    $news->news_id=0;
}

$news_conf= new news_config();
$news_conf->Load();

if($save){
    
    $news->news_title = get_request_variable('title','');
    $news->news_content = get_request_variable('content','');
    $news->news_img = get_request_variable('img','n');
    $day=get_request_variable('day',date('Y-m-d'));
    $news->news_date = parse_date('{Y}-{m}-{d}',$day,false);
    if($news->news_id==0){
       $news->news_time = date('H:i');
    }

    if ($news->news_img !=="n"){
        if ($_FILES['file']['name']!="") {
            if($news->news_id>0){
                $dir= SITE_NEWS_FILE_ROOT.$news->news_stamp.$news->news_exp;
                @unlink ($dir);
                $dir= SITE_NEWS_FILE_ROOT.$news->news_stamp.'_1'.$news->news_exp;
                @unlink ($dir);
            }
            
            $news->news_stamp = time ();
            $img = $_FILES['file']['name'];
            if (preg_match("/\.gif/i",$img)) {$exp=".gif";}
            if (preg_match("/\.png/i",$img)) {$exp=".png";}
            if (preg_match("/\.jpg/i",$img)) {$exp=".jpeg";}
            if (preg_match("/\.jpeg/i",$img)) {$exp=".jpeg";}
            if (!isset($exp)){$exp=".jpg";}
            $news->news_exp=$exp;
            
            $dir= SITE_NEWS_FILE_ROOT.$news->news_stamp.$news->news_exp;
            
            copy($_FILES['file']['tmp_name'], $dir);
            
            if ($exp==".jpeg") {$image = imagecreatefromjpeg($dir);}
            if ($exp==".png") {$image = imagecreatefrompng($dir);}
            if ($exp==".gif") {$image = imagecreatefromgif($dir);}
            $t=getimagesize ($dir);
            $width=$t[0];
            $height=$t[1];
            $y=$news_conf->img_height ;
            $x=$y*($width/$height);
            
            $thumb=imagecreatetruecolor($x,$y);
            imagecopyresampled($thumb,$image,0,0,0,0,$x,$y,$width,$height);
            $dir= SITE_NEWS_FILE_ROOT.$news->news_stamp.'_1'.$news->news_exp;
            $thumb=imagejpeg($thumb, $dir);
            
        }
    }else{
        if($news->news_id>0){
            $dir= SITE_NEWS_FILE_ROOT.$news->news_stamp.$news->news_exp;
            @unlink ($dir);
            $dir= SITE_NEWS_FILE_ROOT.$news->news_stamp."_1".$news->news_exp;
            @unlink ($dir);
        }
        $news->news_exp='.jpg';
        $news->news_stamp=0;
    }

    $errors = $news->IsValidData();
    if (count($errors)==0) {
        $news->news_date = parse_date('{Y}-{m}-{d}',$day,true);
        $db->begin();

        $news->Save();

        $db->commit();
        
        $id='';
        $date=$news->news_date;
    }
}

if($id===''){
    
    if($date){
        $linedate=$date;
    }else{
        $linedate=date('Y-m-d',time());
    }

    $startdate=format_date('{Y}-{m}-01', $linedate);
    $enddate=format_date('{Y}-{m}-31', $linedate);
    
    $month = format_date('{m}', $linedate);
    $year = format_date('{Y}', $linedate);
    
    if (!checkdate ( $month, $day, $year ) && $month!=2)
     $enddate=format_date('{Y}-{m}-30', $linedate);
     else 
     $enddate=format_date('{Y}-{m}-23', $linedate);
    
    
	$query = 'SELECT news_date FROM '.TABLE_NEWS.' WHERE news_date>=\''.$startdate.'\' AND news_date<=\''.$enddate.'\' GROUP BY news_date ';
	$db->query($query);
	$news_dates = $db->value;
    
  	$query = 'SELECT * FROM '.TABLE_NEWS.' WHERE news_date = \''.$linedate.'\' ORDER BY news_id DESC';
  	$db->query($query);
    $newses = $db->value;    
    
    $is_enabled_news_edit=access_enable($STORAGE, $STORAGE['module'], 'edit');
    $is_enabled_news_delete=access_enable($STORAGE, $STORAGE['module'], 'delete');

    include SITE_FILE_ROOT.'template/header.php';
    include SITE_FILE_ROOT.'template/'.$STORAGE['module'].'/'.$STORAGE['action'].'-list.php';
    include SITE_FILE_ROOT.'template/footer.php';   
         
}else{

    $title = 'Редактирование объявления';
    include SITE_FILE_ROOT.'template/header.php';
    include SITE_FILE_ROOT.'template/'.$STORAGE['module'].'/'.$STORAGE['action'].'-id.php';
    include SITE_FILE_ROOT.'template/footer.php';    

}

?>