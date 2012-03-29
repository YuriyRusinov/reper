<div class="pagename">
    <?=$STORAGE['site_path'][count($STORAGE['site_path'])-1]['name']?>
</div>
<?
$as='';
if( count($coms) > 0 ){
  for($j=0;$j<count($coms);$j++){
    if($j== (count($coms)-1)){
      $as.='"'.addslashes($coms[$j]['date']).'"';
    }else{
      $as.='"'.addslashes($coms[$j]['date']).'",';
    }
  };
};
?>
<script language="JavaScript" type="text/javascript">
   arrayStates = [<?=$as?>];
</script>

<button id="edit-role-0" onclick="AddAction(0)" title="Добавить новое сообщение" >Добавить сообщение</button>

<?= $page_break; ?>

<?
if($pattern_flag == false)
{
 echo "<p class='alertnotices'>Формат ввода даты: ГГГГ-ММ-ДД \n Пример: 2011-05-11</p>";
}

?>

<table cellpadding="3" cellspacing="1" border="0" class="list">
<tr>
<td width="4%"> </td>  
        <th>Поиск(дата ГГГГ-ММ-ДД)</th>
        <td width="50%">
            <form action="<?= SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action'].'/'; ?>" method="POST">
                <div id="AutoComplete">
				        <input id="mInput" type="text" name="pattern" value="<?= htmlspecialchars($pattern); ?>" />
				        <div id="Container"></div>
                <input type="submit" name="search" value="GO" />
                </div>
            </form>
        </td>
        <td nowrap><a href="<?= SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action'].'/'; ?>">Показать всех</a></td>
   </tr>
 
   <tr>
        <th>№</th>
        <th> Сообщение</th>
        <th> Категория объекта</th>
        <th> ИКА (отправ) </th>
        <th> ИКА (адресат) </th>
        <th><a href="<?= SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action']; ?>/from/0/sort/date/sort_dir/<?= $sort=='date' ? $convert_sort_dirs[$sort_dir]:'asc'; ?>">Дата отправки</a><?= $sort=='date' ? ($sort_dir=='asc' ? '▲':'▼'):''; ?></th>
        <th> Время </th>
        <th> Дата получения</th>
        <th> Время</th>
       
       
         <th>Операции</th>
        <?
        if(count($coms)>0){
        ?>
        <th> &nbsp; </th>
        <?
        }
        ?>
    </tr>
    <?
        $odd=false;
    
       	
        for ($i=0; $i<count($coms); $i++) {
            $odd=!$odd;
           
            ?>
                <tr <?=($odd?'class="odd"':'')?>>
                    <th><?= htmlspecialchars(trim($coms[$i]['id'])); ?>.</th>
                    <td><?= htmlspecialchars($coms[$i]['message_body']); ?></td>
                    <?
                    $cats_name='';
                    for($j=0;$j<count($cats);$j++)
                    {
                    	if($coms[$i]['id_io_object'] == $cats[$j]->id)
                    	{
                    		$cats_name = $cats[$j]->name;
                    		break;
                    	}
                    }
                    
                    ?>
                    <td><?= htmlspecialchars($cats_name); ?></td>
                    <?
                    $empl_name = '';
                    for($j=0;$j<count($empl);$j++)
                    {
                    	if($coms[$i]['id_dl_sender'] == $empl[$j]->id)
                    	{
                    		$empl_name = $empl[$j]->name;
                    		break;
                    	}
                    }
                    ?>
                    <td><?= htmlspecialchars($empl_name); ?></td>
                    
                    <?
                    for($j=0;$j<count($empl);$j++)
                    {
                    	if($coms[$i]['id_dl_receiver'] == $empl[$j]->id)
                    	{
                    		$empl_name = $empl[$j]->name;
                    		break;
                    	}
                    }
                    ?>
                    <td><?= htmlspecialchars($empl_name); ?></td>
                    
                 
                    <td><?= format_date('{Y}-{m}-{d}', $coms[$i]['sent_datetime']); ?></td>
                    <td><?= format_date('{H}:{i}', $coms[$i]['sent_datetime']); ?></td>
                         
                    <td><?= format_date('{Y}-{m}-{d}', $coms[$i]['receive_datetime']); ?></td>
                    <td><?= format_date('{H}:{i}', $coms[$i]['receive_datetime']); ?></td>
                    
                    <td class="top">
                	<button id="edit-journal-<?= $coms[$i]['id']; ?>" onclick="EditAction(<?= $coms[$i]['id']; ?>)" title="Просмотреть запись"><img src="<?= SITE_URI_ROOT; ?>img/iconDetail.gif" /></button>
 					
                    </td>
           
                    
                </tr>
            <?
        }
    ?>
</table>

<script language="JavaScript" type="text/javascript">
<!--

YAHOO.example.BasicLocal = function() {
    // Use a LocalDataSource
    var oDS = new YAHOO.util.LocalDataSource(arrayStates);
    // Optional to define fields for single-dimensional array
    oDS.responseSchema = {fields : ["state"]};

    // Instantiate the AutoComplete
    var oAC = new YAHOO.widget.AutoComplete("mInput", "Container", oDS);
    oAC.prehighlightClassName = "yui-ac-prehighlight";
    oAC.useShadow = true;
    
    return {
        oDS: oDS,
        oAC: oAC
    };
}();

function EditAction(id)
{
    <?
        if (access_enable($STORAGE, $STORAGE['module'], 'edit')) {
            ?>
                var url = '<?= SITE_HTTP_ROOT; ?><?= $STORAGE['module']; ?>/detail/id/' + id + '/';
                window.location=url;
            <?
        } else {
            ?>
                alert('У Вас нет прав на выполнение операции');
            <?
        }
    ?>
}



function AddAction(id)
{
    <?
        if (access_enable($STORAGE, $STORAGE['module'], 'edit')) {
            ?>
                var url = '<?= SITE_HTTP_ROOT; ?><?= $STORAGE['module']; ?>/edit/id/' + id + '/';
                window.location=url;
            <?
        } else {
            ?>
                alert('У Вас нет прав на выполнение операции');
            <?
        }
    ?>
}



function DeleteAction(id)
{
    <?
        if (access_enable($STORAGE, $STORAGE['module'], 'edit')) {
            ?>
                if (confirm('После проведения операции, восстановление будет невозможно. Вы действительно хотите продолжить?')) {
                    var w = 1;
                    var h = 1;
                    var param = '';
                    var url = '<?= SITE_HTTP_ROOT; ?><?= $STORAGE['module']; ?>/delete/id/' + id + '/callback/Refresh/';
                    OpenPopupWindow(url, w, h, param);
                }
            <?
        } else {
            ?>
                alert('У Вас нет прав на выполнение операции');
            <?
        }
    ?>
}

function Refresh()
{
    var offsetX = document.body.scrollLeft;
    var offsetY = document.body.scrollTop;
    var location = "<?= SITE_HTTP_ROOT.preg_replace('/\/x\/\d+\/y\/\d+/', '', $STORAGE['request_uri']); ?>";

    window.location = location + 'x/' + offsetX + '/y/' + offsetY;
}

//-->
</script>
