<!DOCTYPE html>
<html>
<head>
    <title>POSTリクエストのテスト</title>
</head>
<body>
    <?php
    if ($_SERVER['REQUEST_METHOD'] === 'POST') {
        // POSTリクエストが送信された場合の処理
        $name = $_POST['name'];
        $email = $_POST['email'];
        
        // 受け取ったデータの表示
        echo "名前: " . $name . "<br>";
        echo "メールアドレス: " . $email . "<br>";
    }
    ?>
    
    <form method="post" action="<?php echo $_SERVER['PHP_SELF']; ?>">
        <label for="name">名前:</label>
        <input type="text" id="name" name="name" required><br>
        
        <label for="email">メールアドレス:</label>
        <input type="email" id="email" name="email" required><br>
        
        <input type="submit" value="送信">
    </form>
</body>
</html>
