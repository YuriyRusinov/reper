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

<button id="edit-role-0" onclick="EditAction(<?= $id; ?>, 0, 0)" title="Добавить новую запись" >Добавить запись</button>


<table cellpadding="3" cellspacing="1" border="0" class="list">
<tr>
  
    <tr>
         <?
         
function String2File($sIn, $sFileOut) {
  						$rc = false;
  												do {
    if (!($f = fopen($sFileOut, "wa+"))) {
      $rc = 1; break;
    }
    if (!fwrite($f, $sIn)) {
      $rc = 2; break;
    }
    $rc = true;
  } while (0);
  if ($f) {
    fclose($f);
  }
  return ($rc);
} 
         
              for ($i=0; $i<count($attr_title); $i++) {

            		 echo "<th>".$attr_title[$i]."</th>";
            		 echo "<th>"." "."</th>";
            		 
              }
            
        ?>
        <?
        if(count($records)>0){
        ?>
        <th> &nbsp; </th>
        <?
        }
        ?>
    </tr>
    <?
        $odd=false;
          	
        for ($i=0; $i<count($records); $i++) {
            $odd=!$odd;
           
            ?>
                <tr <?=($odd?'class="odd"':'')?>>
                     
                  
                     <?
        
           
            for($j=0;$j<count($data); $j++) {
            
            	//generic image file
            	if($attr_ids[$j] == 15)
            	{
                 
                     //get jpeg file
          
                    $filename = SITE_FILE_ROOT.'data/'.session_id().$records[$i]['id'].$j.'.jpg';
          
                 
          
                   $raw = pg_unescape_bytea($records[$i][$data[$j]]);
          
                     file_put_contents ( $filename, $raw );
            		  
                  
            		$img = "<td> <img src='".SITE_URI_ROOT."data/".session_id().$records[$i]['id'].$j.".jpg'> </td>";
                 	echo $img;
            		
            	}
            	else {
            		
            	 echo "<td>".$records[$i][$data[$j]]."</td>";
            	}
            	 
            	 if($data[$j] != 'id')
            	 {
            	 	if($attr_ids[$j] == 15)
            	 	{
            	 		?>
            	 		
            	 		<td class="top">
                	
                    </td> 
            	 		<?
            	 		
            	 	}
            	 	else {
            	 ?>
            	    <td class="top">
                	 <button id="edit-journal-<?= $records[$i]['id']; ?>" onclick="EditAction(<?= $id; ?>,<?= $records[$i]['id']?>, '<?= $data[$j];?>')" title="Редактировать запись"><img src="<?= SITE_URI_ROOT; ?>img/iconEdit.gif" /></button>			
                    </td>   
            	 <?
            	  }
            	 }
            	 else 
            	 {
            	 	?>
            	 	<td> </td>
            	 	<?
            	 }
            	 
            }
        
        
        
        ?>
                        
          
           
                                  
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

function EditAction(id,attr_id,name)
{
    <?
        if (access_enable($STORAGE, $STORAGE['module'], 'edit')) {
            ?>
                var w = 550;
                var h = 200;
                var param = '';
                var url = '<?= SITE_HTTP_ROOT; ?><?= $STORAGE['module']; ?>/edit_tables/id/' + id + '/rec_id/' + attr_id + '/name/' + name + '/callback/Refresh/';
                OpenPopupWindow(url, w, h, param);
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
