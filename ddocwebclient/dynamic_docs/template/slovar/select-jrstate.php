<?
if($_REQUEST['callback'])
{
	$funcv = $_REQUEST['callback'];

	$_SESSION['func'] = $funcv;
}
?>




<table cellpadding="3" cellspacing="1" border="0" class="list" width="100%">
    <tr>
        <th>Поиск</th>
        <td width="100%">
            <form action="<?= SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action'].'/'; ?>" method="POST">
                <input type="hidden" name="callback" value="<?= htmlspecialchars($_REQUEST['callback']); ?>" />
                <input type="text" name="pattern" value="<?= htmlspecialchars($pattern); ?>" />
                <input type="submit" name="search" value="GO" />
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
    	?>

    	var cmd = "window.opener.<?= $fc; ?>(id, name);";
        
    	
        eval(cmd);
        element.className="selected";
    }

    return false;
}
//-->
</script>