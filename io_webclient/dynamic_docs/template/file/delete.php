<?
    if (!$errors) {
?>
<script language="JavaScript">
<!--
    var cmd = "window.opener.<?= $_REQUEST['callback']; ?>('<?= htmlspecialchars($source); ?>');";
    eval(cmd);
    self.close();
//-->
</script>
<?
    }
?>