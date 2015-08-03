<?
if($_REQUEST['callback'])
{
	$funcv = $_REQUEST['callback'];

	$_SESSION['func'] = $funcv;
}

if($save)
{
	?>
}
<script language="JavaScript">
<!--
    var cmd = 'window.opener.<?= $_REQUEST['callback']; ?>(1,1,\'delete\');';
    eval(cmd);
    self.close();
//-->
</script>
<?
}
?>




<table cellpadding="3" cellspacing="1" border="0" class="list" width="100%">
    <tr>
        <th>Поиск</th>
        <td width="100%">
            <form name="editform" action="<?= SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action'].'/'; ?>" method="POST">
                <input type="hidden" name="callback" value="<?= htmlspecialchars($_REQUEST['callback']); ?>" />
                 <input type="hidden" name="cat_id" value="<?= htmlspecialchars($_REQUEST['cat_id']); ?>" />
                <input type="text" name="pattern" value="<?= htmlspecialchars($pattern); ?>" />
                <input type="submit" name="search" value="GO" />
                <input type="hidden" name="io_id" value="" />
				<input type="hidden" name="save" value="1" />		                
            </form>
        </td>
        <td nowrap><a href="<?= SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action'].'/callback/'.urlencode($_REQUEST['callback']); ?>">Показать всех</a></td>
    </tr>
    <tr>
        <th>По алфавиту</th>
        <td colspan="2">
            <?
                for ($i=97;$i<123;$i++) {
                    ?>
                        <a href="<?= SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action'].'/letter/'.urlencode(chr($i)).'/callback/'.urlencode($_REQUEST['callback']); ?>" class="capitalize<?= $letter==chr($i) ? ' selected':''; ?>"><?= htmlspecialchars(chr($i)); ?></a>
                    <?
                }
            ?><br>
            <?
                for ($i=1072;$i<1103;$i++) {
                    ?>
                        <a href="<?= SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action'].'/letter/'.urlencode(ords2unicode(array($i))).'/callback/'.urlencode($_REQUEST['callback']); ?>" class="capitalize<?= $letter==chr($i) ? ' selected':''; ?>"><?= ords2unicode(array($i)); ?></a>
                    <?
                }
            ?>
        </td>
    </tr>
    <tr>
    
    		    		
			
			
    </tr>
    
    
</table><br>


<br/>


<script type="text/javascript" language="JavaScript">
<!--





//-->
</script>

<?
if(count($contact_persons) == 0)
{
?>	
<table cellpadding="3" cellspacing="1" border="0" class="list" width="100%">
	<th>
		Инфомационных объектов заданной категории в системе не существует!!!
    </th>
 </table>
<?	
}

?>

<table cellpadding="3" cellspacing="1" border="0" class="list" width="100%">
    <?
        for ($i=0;$i<count($contact_persons);$i++) {
            ?>
                <tr>
                    <td<?= ($i % 2 ? ' class="dark"':''); ?>><a href="" onclick="return OpenerCall(this, '<?= escape($contact_persons[$i]->id); ?>', '<?= escape($contact_persons[$i]->name); ?>')"><?= htmlspecialchars($contact_persons[$i]->name); ?></a></td>
                </tr>
            <?
        }
    ?>
</table>




<script type="text/javascript" language="JavaScript">
<!--
function OpenerCall(element, id, name)
{

	if (window.opener) {
    	
    	<?
    	$fc = $_REQUEST['callback'];
    	
    	if(!$fc)
    	{
    		$fc = $_SESSION['func'];
    	}
    	
    	
    	$cat_id = $_REQUEST['cat_id'];
    	
    	if(!$cat_id)
    	{
    		$cat_id = $_SESSION['cat_id'];
    	}
    	
    	
    	?>
    	

    	document.editform.elements['io_id'].value = id;
    	element.className="selected";
    	
    	document.editform.submit();
    	
    	
    	/*var cmd = "window.opener.<?= $fc; ?>(id, <?= $cat_id;?>);";
        
    	
        eval(cmd);*/
    	
    	
        element.className="selected";
    }

    return false;
}
//-->
</script>