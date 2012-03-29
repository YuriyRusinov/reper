<link rel="STYLESHEET" type="text/css" href="<?= SITE_URI_ROOT; ?>js/dhtmlx/dhtmlxtree.css">
<script type="text/javascript" src="<?= SITE_URI_ROOT; ?>js/dhtmlx/dhtmlxcommon.js"></script>
<script type="text/javascript" src="<?= SITE_URI_ROOT; ?>js/dhtmlx/dhtmlxtree.js"></script>
<script type="text/javascript" src="<?= SITE_URI_ROOT; ?>js/dhtmlx/ext/dhtmlxtree_xw.js"></script>
<script type="text/javascript" src="<?= SITE_URI_ROOT; ?>js/dhtmlx/ext/dhtmlxtree_my.js"></script>

<div id="department-tree" class="tree"></div>

<script type="text/javascript" language="JavaScript">
<!--

function SelectDepartment(itemId) {
    if (window.opener) {
        var id = itemId;
        var name = tree.getItemText(itemId);
        var cmd = 'window.opener.<?= $_REQUEST['callback']; ?>(id, name);';
        eval(cmd);
    }

    return false;
}

tree=new dhtmlXTreeObject("department-tree","100%","100%","root");
tree.attachEvent("onClick", SelectDepartment);
tree.attachEvent("onDblClick", SelectDepartment);

tree.setImagePath("<?= SITE_URI_ROOT; ?>js/dhtmlx/imgs/csh_vista/");
tree.enableCheckBoxes(false);
tree.enableTreeLines(true);
tree.enableHighlighting(false);
tree.enableActiveImages(false);
tree.enableDragAndDropScrolling(false);
tree.setXMLAutoLoading("<?= SITE_HTTP_ROOT; ?>directory.php?module=rubrics&view=tree");
tree.attachEvent("onXLE", RestoreTree);
tree.loadXML("<?= SITE_HTTP_ROOT; ?>directory.php?module=rubrics&view=tree&id=root");


function RestoreTree(treeObj)
{
    treeObj.attachEvent("onXLE", null);
    treeObj.loadOpenStates('department-tree');
}
window.onunload = new Function('tree.saveOpenStates("department-tree");');

//-->
</script>
