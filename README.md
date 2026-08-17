# php-usdt-bridge

`call_usdt_bridge(string $message)` を提供する PHP 拡張です。呼び出されると
SystemTap の USDT (Userspace Statically Defined Tracing) プローブを発火し、
以下の3つの引数を渡します。

- `call_usdt_bridge` に渡された文字列
- 呼び出し元スクリプトのファイル名（実行時に取得）
- 呼び出し元の行番号（実行時に取得）

- プローブ (provider) 名: `usdt_bridge`
- トレースポイント (probe) 名: `call`

PHP 7 / PHP 8 の両方に対応しています（同一のソースでビルド可能です）。

## 必要なもの

- PHP の開発用ヘッダ一式と `phpize` / `php-config`
  - Debian/Ubuntu: `sudo apt install php-dev`
  - RHEL/CentOS/Fedora: `sudo dnf install php-devel`
- SystemTap の USDT ヘッダ (`sys/sdt.h`)
  - Debian/Ubuntu: `sudo apt install systemtap-sdt-dev`
  - RHEL/CentOS/Fedora: `sudo dnf install systemtap-sdt-devel`
- SystemTap 本体（プローブを実際に観測する場合）
  - Debian/Ubuntu: `sudo apt install systemtap`
  - RHEL/CentOS/Fedora: `sudo dnf install systemtap`
- C コンパイラ・make などのビルドツール一式

`phpize` / `php-config` は使用する PHP のバージョン（7系 or 8系）のものを
使ってください。複数バージョンの PHP が入っている環境では、対象バージョンの
`phpize` / `php-config` をフルパスで指定するか、`PATH` を切り替えてビルドし
てください。

## ビルド方法

```sh
phpize
./configure --enable-usdt-bridge
make
sudo make install
```

`make install` は `usdt_bridge.so` を、使用している PHP の拡張ディレクトリ
（`php-config --extension-dir` の出力先）にインストールします。

特定バージョンの `phpize` / `php-config` を明示したい場合は以下のように指定
します。

```sh
phpize --with-php-config=/usr/bin/php-config8.2
./configure --enable-usdt-bridge --with-php-config=/usr/bin/php-config8.2
make
sudo make install
```

PHP 7 系と PHP 8 系の両方に導入する場合は、それぞれの `phpize` /
`php-config` を使って上記のビルドを個別に行ってください（ビルド成果物は
バージョンごとに別物になります）。ビルドが終わったら一旦生成物をクリーンに
してから別バージョン向けにビルドし直してください。

```sh
make clean
phpize --clean
```

## php.ini への設定

インストール後、使用する PHP の `php.ini`（または
`/etc/php/<version>/mods-available/usdt_bridge.ini` のような専用ファイル）
に以下を追加します。

```ini
extension=usdt_bridge.so
```

設定後、`php -m | grep usdt_bridge` で拡張が読み込まれていることを確認でき
ます。

## 使い方

```php
<?php
call_usdt_bridge("hello from php");
```

このスクリプトを実行すると、`usdt_bridge:call` プローブが以下の引数で発火
します。

1. `"hello from php"`
2. このスクリプトのファイルパス
3. `call_usdt_bridge(...)` が呼ばれた行番号

## bpftrace で観測する

```sh
sudo bpftrace -e 'usdt:./modules/usdt_bridge.so:usdt_bridge:call {printf("[%s] pid=%-6d comm=%-16s message=%s file=%s line=%d\n", strftime("%H:%M:%S", nsecs), pid, comm, str(arg0), str(arg1), arg2); }'
```

別ターミナルで拡張を読み込んだPHPを実行すると、メッセージ・ファイル名・行
番号・PID・コマンド名が出力されます。

```sh
php -d extension=./modules/usdt_bridge.so -r 'call_usdt_bridge("hello");'
```
