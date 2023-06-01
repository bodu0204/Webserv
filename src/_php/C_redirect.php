<?php
// リダイレクト先のURL
$redirect_url = "http://example.com/new-page.php";

// リダイレクトを実行
header("Location: " . $redirect_url);
exit;
?>
