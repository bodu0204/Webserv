<!DOCTYPE html>
<html>
<head>
    <title>Cookieの利用</title>
</head>
<body>
    <?php
    // Cookieの設定
    $cookieName = "username";
    $cookieValue = "JohnDoe";
    $expiration = time() + (60 * 60 * 24 * 7); // 1週間の有効期限

    setcookie($cookieName, $cookieValue, $expiration);

    // Cookieの取得と表示
    if (isset($_COOKIE[$cookieName])) {
        $username = $_COOKIE[$cookieName];
        echo "<p>ようこそ、$username さん！</p>";
    } else {
        echo "<p>初めての訪問ですね！</p>";
    }

    // Cookieの削除
    if (isset($_COOKIE[$cookieName])) {
        setcookie($cookieName, "", time() - 3600);
        echo "<p>Cookieを削除しました。</p>";
    }
    ?>
</body>
</html>
