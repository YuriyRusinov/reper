<?
if($_REQUEST['callback'])
{
	$funcv = $_REQUEST['callback'];

	$_SESSION['func'] = $funcv;
}
?>

<link rel="STYLESHEET" type="text/css" href="<?= SITE_URI_ROOT; ?>js/dhtmlx/dhtmlxtree.css" />
<script type="text/javascript" src="<?= SITE_URI_ROOT; ?>js/dhtmlx/dhtmlxcommon.js"></script>
<script type="text/javascript" src="<?= SITE_URI_ROOT; ?>js/dhtmlx/dhtmlxtree.js"></script>
<script type="text/javascript" src="<?= SITE_URI_ROOT; ?>js/dhtmlx/ext/dhtmlxtree_xw.js"></script>
<script type="text/javascript" src="<?= SITE_URI_ROOT; ?>js/dhtmlx/ext/dhtmlxtree_my.js"></script>


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


<div id="department-tree" class="tree"></div>
<br/>
<script type="text/javascript" language="JavaScript">
<!--

function SelectDepartment(itemId) {
    var url = '<?= SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action'].'/callback/'.urlencode($_REQUEST['callback']); ?>';
    if (itemId > 0) {
         var itemId = tree.getSelectedItemId();
         var parentId = tree.getParentId(itemId);
         url = url +'/dept_id/' + itemId;        
    }
    location.href=url;
}

//-->
</script>

<script type="text/javascript" language="JavaScript">
<!--
tree=new dhtmlXTreeObject("department-tree","100%","100%","root");
tree.attachEvent("onDblClick", SelectDepartment);
tree.enableDragAndDrop(false);
tree.enableImageDrag(false);
tree.setImagePath("<?= SITE_URI_ROOT; ?>js/dhtmlx/imgs/csh_vista/");
tree.enableCheckBoxes(false);
tree.enableTreeLines(true);
tree.enableHighlighting(false);
tree.enableActiveImages(true);
tree.enableDragAndDropScrolling(true);
tree.setXMLAutoLoading("<?= SITE_HTTP_ROOT; ?>directory.php?module=department&view=tree");
tree.attachEvent("onXLE", RestoreTree);
tree.loadXML("<?= SITE_HTTP_ROOT; ?>directory.php?module=department&view=tree&id=root");


function RestoreTree(treeObj)
{
    treeObj.attachEvent("onXLE", null);
    treeObj.loadOpenStates('department-view-tree');
}
window.onunload = new Function('tree.saveOpenStates("department-view-tree");');


//-->
</script>



<table cellpadding="3" cellspacing="1" border="0" class="list" width="100%">
    <?
        for ($i=0;$i<count($contact_persons);$i++) {
            ?>
                <tr>
                    <td<?= ($i % 2 ? ' class="dark"':''); ?>><a href="" onclick="return OpenerCall(this, '<?= escape($contact_persons[$i]->id_employee); ?>', '<?= escape($contact_persons[$i]->name); ?>')"><?= htmlspecialchars($contact_persons[$i]->name); ?></a></td>
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