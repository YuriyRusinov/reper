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

<button id="edit-role-0" onclick="EditAction(0)" title="Добавить новый файл" >Добавить файл</button>

<?= $page_break; ?>
<input type="hidden" name="id" value="<?=$id?>" />
<table cellpadding="3" cellspacing="1" border="0" class="list">
<tr>
<td width="4%"> </td>  
        <th>Поиск( название)</th>
        <td width="50%">
            <form action="<?= SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action'].'/'; ?>" method="POST">
                <div id="AutoComplete">
				        <input id="mInput" type="text" name="pattern" value="<?= htmlspecialchars($pattern); ?>" />
				        <div id="Container"></div>
                <input type="submit" name="search" value="GO" />
                </div>
            </form>
        </td>
         </tr>     
    <tr>
        <th>№</th>
        <th> Название</th>
        <th> Описание</th>
         <th> Категория </th>
        
        <th> Приложение Windows </th>
       
        <th> Приложение МСВС</th>
        <th> Расширение</th>
       
        
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
                    <td><?= htmlspecialchars($coms[$i]['id_url']); ?></td>
                    <td><?= htmlspecialchars($coms[$i]['uo_name']); ?></td>
                   
                    <td><?= htmlspecialchars($coms[$i]['iu_name']); ?></td>
                    <td><?= htmlspecialchars($coms[$i]['win_app']); ?></td>                    
                  
                     <td><?= htmlspecialchars($coms[$i]['lin_app']); ?></td>  
                    <td><?= htmlspecialchars($coms[$i]['src_ext']); ?></td>
                    
                    <td class="top">
                	<button id="edit-journal-<?= $coms[$i]['id']; ?>" onclick="EditAction(<?= $coms[$i]['id']; ?>)" title="Редактировать запись"><img src="<?= SITE_URI_ROOT; ?>img/iconEdit.gif" /></button>
 					<button id="delete-journal-<?= $coms[$i]['id']; ?>" onclick="DeleteAction(<?= $coms[$i]['id']; ?>)" title="Удалить запись" ><img src="<?= SITE_URI_ROOT; ?>img/iconDelete.gif" /></button>
 					<button id="view-journal-<?= $coms[$i]['id']; ?>" onclick="LogAction(<?= $coms[$i]['id']; ?>)" title="Просмотреть логи" ><img src="<?= SITE_URI_ROOT; ?>img/iconClock.gif" /></button>
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

function LogAction(id)
{
    <?
        if (access_enable($STORAGE, $STORAGE['module'], 'edit')) {
            ?>
                var url = '<?= SITE_HTTP_ROOT; ?><?= $STORAGE['module']; ?>/log/id/' + id + '/';
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
