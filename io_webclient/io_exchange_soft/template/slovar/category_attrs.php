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
        
<?


for($i=0;$i<count($coms[0]);$i++)
{
	

                     $cat_name = '';
                     for($j=0;$j<count($names);$j++)
                     {
                     
                     	if( $names[$j]->id === $coms[0][$i]['id_io_attribute'])
                     	{
                     	
                     		$cat_name = $names[$j]->name;
                     		break;	
                     	}
                     }
                     ?>
                     
                     
                     <th><?= htmlspecialchars($cat_name); ?></th>
                     <?
	
}

?>
        <th> Редактировать </th>
        
               
        <?
        if(count($coms[0])>0){
        ?>
        <th> &nbsp; </th>
        <?
        }
        ?>
    </tr>
    
    <?
    
    for($j=0; $j<count($coms); $j++)
    {
    
    ?>
    
    
                <tr <?=($odd?'class="odd"':'')?>>
                
                   <th><?= htmlspecialchars($j+1); ?>.</th>
    <?
        $odd=false;
          	
                
        for ($i=0; $i<count($coms[$j]); $i++) {
            $odd=!$odd;
           
            ?>
            
                  
                  
                     
                     
                     <td><p id="value-<?= $coms[$j][$i]['id_io_attribute']; ?>" > <?= htmlspecialchars($coms[$j][$i]['value']); ?></p> </td>
                    
                    
                                
                
                
            <?
        }
    ?>
    
    <td class="top">
                	<button id="edit-journal-<?= $coms[$j][0]['id_io_object']; ?>" onclick="EditAction(<?= $coms[$j][0]['id_io_object']; ?>)" title="Редактировать информационный объект"><img src="<?= SITE_URI_ROOT; ?>img/iconEdit.gif" /></button>
 					
 					
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
                var w = 550;
                var h = 200;
                var param = '';
                var url = '<?= SITE_HTTP_ROOT; ?><?= $STORAGE['module']; ?>/edit/id/' + id + '/';
                //OpenPopupWindow(url, w, h, param);
                
                window.location = url;
            <?
        } else {
            ?>
                alert('У Вас нет прав на выполнение операции');
            <?
        }
    ?>
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
