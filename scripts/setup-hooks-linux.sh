#!/bin/sh

HOOK_SRC="../scripts/commit-msg"
HOOK_DEST="../.git/hooks/commit-msg"

# 删除旧的 hook
[ -f "$HOOK_DEST" ] && rm "$HOOK_DEST"

# 创建软链接
ln -s "../$HOOK_SRC" "$HOOK_DEST"
chmod +x "$HOOK_DEST"

echo "hooks 已安装成功"
