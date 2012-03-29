<?
    if (!$errors) {
?>
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