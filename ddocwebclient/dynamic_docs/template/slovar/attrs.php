<div class="pagename">
    <?=$STORAGE['site_path'][count($STORAGE['site_path'])-1]['name']?>
</div>
<?
$as='';

?>
<script language="JavaScript" type="text/javascript">
   arrayStates = [<?=$as?>];
</script>

<?= $page_break; ?>
  <input type="hidden" name="id" value="<?=$id?>" />
<table id="editform" cellpadding="3" cellspacing="1" border="0" class="list" name="editform">
<tr>
<td width="4%"> </td>  
        <th>Поиск( код)</th>
        <td width="50%">
            <form action="<?= SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action'].'/'; ?>" method="POST" name="editform">
            
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
        
        <th> Атрибут</th>
        <th> Тип</th>
        <th> Значение </th>
        
        
        
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
                     <th><?= htmlspecialchars($i+1); ?>.</th>
                    <?
                     $cat_name = '';
                     for($j=0;$j<count($names);$j++)
                     {
                     
                     	if( $names[$j]->id === $coms[$i]['id_io_attribute'])
                     	{
                     	
                     		$cat_name = $names[$j]->name;
                     		break;	
                     	}
                     }
                     ?>
                     
                     
                     <td><?= htmlspecialchars($cat_name); ?></td>
                     
                     
                      <?
                     $cat_type = '';
                     for($j=0;$j<count($cats);$j++)
                     {
                     	
                     	if($cats[$j]->id === $coms[$i]['id_attr_type'])
                     	{
                     		
                     		$cat_type = $cats[$j]->name;
                     		break;
                     	}
                     	
                     }

                     
?>                     
          
          
                        <td><?= htmlspecialchars($cat_type); ?></td> 
                     
                     
                        <? if($coms[$i]['id_attr_type'] == 2)
                        {
                        	?>
                        	
                        	<td><p id="value-<?= $coms[$i]['id_io_attribute']; ?>" > <?= htmlspecialchars($coms[$i]['value']); ?></p> </td>
                        <?
						}
                        else 
                        {
                        	?>
                        
                            <td><p id="value-<?= $coms[$i]['id_io_attribute']; ?>" > <?= htmlspecialchars($coms[$i]['value']); ?></p> </td>
                     <?
                        }
                        ?>
                     
                     
                     
                    <td class="top">
                	<button id="edit-journal-<?= $coms[$i]['id_io_attribute']; ?>" onclick="EditAction(<?= $coms[$i]['id_io_object']; ?>,<?= $coms[$i]['id_io_attribute']?>)" title="Редактировать запись"><img src="<?= SITE_URI_ROOT; ?>img/iconEdit.gif" /></button>
 					
 					
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

function EditAction(id,attr_id)
{
    <?
        if (access_enable($STORAGE, $STORAGE['module'], 'edit')) {
            ?>
                var w = 550;
                var h = 200;
                var param = '';
                var url = '<?= SITE_HTTP_ROOT; ?><?= $STORAGE['module']; ?>/edit_attr/id/' + id + '/attr_id/' + attr_id + '/callback/SetValue';
                OpenPopupWindow(url, w, h, param);
            <?
        } else {
            ?>
                alert('У Вас нет прав на выполнение операции');
            <?
        }
    ?>
}


function SetValue(attr_id, text)
{
	
	
	var param = 'value-' + attr_id;	
	var obj =  document.getElementById(param);
		
	var parsedText = document.createTextNode (text);
    obj.innerHTML = "";
    obj.appendChild (parsedText);
}


function DeleteAction(id,attr_id)
{
    <?
        if (access_enable($STORAGE, $STORAGE['module'], 'edit')) {
            ?>
                if (confirm('После проведения операции, восстановление будет невозможно. Вы действительно хотите продолжить?')) {
                    var w = 550;
                    var h = 200;
                    var param = '';
                    var url = '<?= SITE_HTTP_ROOT; ?><?= $STORAGE['module']; ?>/delete_attr/id/' + id + '/attr_id/' + attr_id + '/callback/Refresh/';
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
