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



<?= $page_break; ?>
<table cellpadding="3" cellspacing="1" border="0" class="list">
<tr>
<td class="top">
                	<button id="edit-journal-<?= $id; ?>" onclick="EditAction(<?= $id; ?>)" title="Вернуться к редактированию ИО"><img src="<?= SITE_URI_ROOT; ?>img/iconEdit.gif" /></button>
 					
</td>   

<td width="4%"> </td>  
        <th>Поиск(дата ГГГГ-ММ-ДД)</th>
        <td width="50%">
            <form action="<?= SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action'].'/'; ?>" method="POST">
                <div id="AutoComplete">
				        <input id="mInput" type="text" name="pattern" value="<?= htmlspecialchars($pattern); ?>" />
				        <div id="Container"></div>
                <input type="submit" name="search" value="GO" />
                </div>
                <input type="hidden" name="id" value="<?= htmlspecialchars($id); ?>" />
            </form>
        </td>
        <td nowrap><a href="<?= SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action'].'/'; ?>">Показать все</a></td>
           
   </tr>     
    <tr>
        <th>№</th>
        <th width="30%"> Пользователь</th>
        <th width="30%"> Тип операции</th>
        
        <th><a href="<?= SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action']; ?>/from/0/sort/date/sort_dir/<?= $sort=='date' ? $convert_sort_dirs[$sort_dir]:'asc'; ?>">Дата изменений в БД</a><?= $sort=='date' ? ($sort_dir=='asc' ? '▲':'▼'):''; ?></th>
       
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
                    <td><?= htmlspecialchars($coms[$i]['user_name']); ?></td>
                    
                     <? 
                    $cat_name = $coms[$i]['oper_type'];
                    
                    	switch (trim($cat_name))
                    	{
                    		case '1': $cat_name = "Добавление новой записи в журнал учета входящих распоряжений"; break;
                    		case '2': $cat_name = "Добавление новой записи в табель срочных донесений"; break;
                    		case '3': $cat_name = "Создание документа"; break;
                    		case '4': $cat_name = "Создание справочника"; break;
                    		case '5': $cat_name = "Создание новой записи справочника"; break;
                    		
                    		default: $cat_name = "Тип операции не установлен.Обратитесь к администратору.";
                    	}
                    ?>
                    <td><?= htmlspecialchars($cat_name); ?></td>
                 
                    <td><?= format_date('{Y}-{m}-{d}', $coms[$i]['datetime']); ?></td>
                                
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



function Refresh()
{
    var offsetX = document.body.scrollLeft;
    var offsetY = document.body.scrollTop;
    var location = "<?= SITE_HTTP_ROOT.preg_replace('/\/x\/\d+\/y\/\d+/', '', $STORAGE['request_uri']); ?>";

    window.location = location + 'x/' + offsetX + '/y/' + offsetY;
}

//-->
</script>
