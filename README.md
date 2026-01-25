# FileModWatch

[日本語](#filemodwatch-ja) | [English](#filemodwatch-en)

---

## FileModWatch (JA)

Windows（MFC）製の小さな常駐ツールです。  
指定フォルダ内のファイル変更を監視し、変更があったファイルを別フォルダへ自動コピーします。  
**最小化するとシステムトレイに入る**タイプです。

> このリポジトリは **ソースコード（プロジェクト一式）** のみを公開します。  
> バイナリ配布は行いません。使いたい方は各自でビルドしてください。

### できること
- 指定フォルダ直下のファイル変更を検知して、別フォルダにコピー
- 監視対象ファイルをワイルドカードで指定（複数指定可）
  - 例：`*.txt;*.csv;*.log`
- 最小化でシステムトレイ常駐（トレイメニューから表示/終了）
- Win32(x86) / x64 の両方でビルド可能

### 動作環境（目安）
- 開発当初：Windows 7 向け
- 現在：Windows 10 で継続利用
- Windows 11：**VS2022 でビルド＆実行できることを確認（作者環境）**
- Windows 11 は 64bit のため **x64 推奨**

### 使い方（概要）
1. 起動
2. `Directory`（監視元フォルダ）と `Copy to`（コピー先フォルダ）を設定
3. `Watch` に対象パターンを入力（任意）
   - 例：`*.txt;*.csv`
4. `Start` で監視開始（`Stop` で停止）
5. 最小化するとシステムトレイに入ります  
   - トレイアイコンのメニューから表示/終了できます

### 監視・コピーの仕様（重要）
- 監視対象は **指定フォルダ直下のみ**（サブフォルダは対象外）
- 変更検知のトリガは OS 通知（`FindFirstChangeNotification`）を利用
- 変更検知後、安定化のため **約3秒待って**からファイル一覧を再スキャンし差分判定します
- コピーは **上書き**です（同名ファイルがあれば置き換えます）
- 削除は同期しません（コピー先の削除は行いません）

### 設定ファイル（.watch）
本アプリは MFC のドキュメント形式として `.watch` を扱います。  
監視設定を保存して再利用できます（MRU/関連付けは環境により動作）。

### ビルド方法（Visual Studio）
このリポジトリは **ソースコード（プロジェクト一式）** を公開しています。バイナリ配布は行いません。  
利用する場合は各自でビルドしてください。

1. `FileModWatch.sln` を Visual Studio で開く
2. 構成（Debug/Release）とプラットフォーム（Win32/x64）を選択
3. ビルド

#### x86/x64 × Debug/Release をまとめてビルドする
Visual Studio の **バッチ ビルド (Batch Build)** を使うと、4種類まとめてビルドできます。

- Debug | Win32
- Release | Win32
- Debug | x64
- Release | x64

（メニュー例）`ビルド` → `バッチ ビルド...`

### 注意 / 既知の制限
- コピー先を監視対象に含めると、コピーがトリガになってループする可能性があります
- ロック中ファイルはコピーに失敗する場合があります
- ネットワーク共有（UNCパス等）は環境によって挙動が異なる可能性があります
- レジストリに設定/MRU等を保存します（アプリ側で `Comona` をキーに使用）

### カスタマイズについて
本ツールは現状渡し（as-is）で公開します。  
要件の違いで「ここが合わない」という部分は当然出てくると思いますので、**C++（cpp）を編集できる方は自由に改造して使ってください**。

改善提案やPRは歓迎しますが、作者が積極的にメンテナンスする予定はありません。  
READMEは必要に応じて追記していきます。

### License
MIT License. See `LICENSE`.

---

## FileModWatch (EN)

A small Windows resident utility (MFC).  
It watches a folder for file changes and automatically copies changed files to another folder.  
When minimized, it lives in the **system tray**.

> This repository publishes **source code only** (full Visual Studio project/solution).  
> No binaries are provided. Please build it yourself.

### Features
- Detects changes in files under the specified folder and copies them to a destination folder
- File filters with wildcards (multiple patterns separated by semicolons)
  - e.g. `*.txt;*.csv;*.log`
- Minimizes to the system tray (restore/exit from tray menu)
- Supports both Win32(x86) and x64 builds

### System Requirements (rough)
- Originally developed for Windows 7
- Actively used on Windows 10
- Windows 11: confirmed build & run with **Visual Studio 2022** (author environment)
- Since Windows 11 is **64-bit only**, **x64 is recommended**

### Usage (quick)
1. Launch the app
2. Set `Directory` (source folder) and `Copy to` (destination folder)
3. (Optional) Set `Watch` patterns
   - e.g. `*.txt;*.csv`
4. Click `Start` to begin watching (`Stop` to stop)
5. Minimize to system tray  
   - Restore/exit from the tray icon menu

### Watch/Copy behavior (important)
- Watches **only the top-level folder** (subfolders are NOT included)
- Uses Windows change notification (`FindFirstChangeNotification`)
- After a change is detected, it waits about **3 seconds** and then rescans files to compute differences
- Copy is **overwriting** (existing files with the same name will be replaced)
- Deletions are not synchronized (it never deletes files in the destination)

### Settings file (.watch)
The app uses `.watch` as an MFC document format.  
You can save and reopen your watch settings (MRU/file association depends on the environment).

### Build (Visual Studio)
This repository publishes **source code only**. No binaries are provided.  
Please build it yourself.

1. Open `FileModWatch.sln` with Visual Studio
2. Select configuration (Debug/Release) and platform (Win32/x64)
3. Build

#### Build all 4 variants at once
Use Visual Studio **Batch Build** to build all combinations:

- Debug | Win32
- Release | Win32
- Debug | x64
- Release | x64

(Menu) `Build` → `Batch Build...`

### Notes / Known limitations
- Avoid watching the destination folder to prevent copy-trigger loops
- Copy may fail when a file is locked by another process
- Network shares (UNC paths) may behave differently depending on your environment
- The app stores settings/MRU in the registry (uses `Comona` as a key)

### Customization
This project is published **as-is**. Requirements vary, so some behavior may not match your needs.  
If you can modify C++ code, feel free to customize it. Feedback and PRs are welcome, but active maintenance is not guaranteed.  
The README will be updated when needed.

### License
MIT License. See `LICENSE`.
