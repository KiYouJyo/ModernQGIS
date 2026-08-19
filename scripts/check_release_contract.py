#!/usr/bin/env python3
import json,re,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
def fail(msg): print(f'release contract failed: {msg}',file=sys.stderr); raise SystemExit(1)
def main():
    cmake=(ROOT/'CMakeLists.txt').read_text(encoding='utf-8'); match=re.search(r'project\(\s*ModernQGIS\s+VERSION\s+([0-9]+\.[0-9]+\.[0-9]+)',cmake,re.S)
    if not match: fail('cannot read ModernQGIS VERSION from CMakeLists.txt')
    manifest=json.loads((ROOT/'release/release.json').read_text(encoding='utf-8')); version=manifest.get('version')
    if version!=match.group(1): fail(f'CMake version {match.group(1)} != release version {version}')
    if manifest.get('product')!='ModernQGIS': fail('unexpected product name')
    if manifest.get('qgisBundled') is not False: fail('ModernQGIS must not vendor or bundle QGIS')
    locales=['zh-CN','ja-JP','en-US']
    if manifest.get('locales')!=locales: fail(f'locales must be exactly {locales}')
    for locale in locales:
        note=ROOT/f'release/notes/v{version}.{locale}.md'
        if not note.is_file() or not note.read_text(encoding='utf-8').strip(): fail(f'missing release note {note.relative_to(ROOT)}')
    github=manifest.get('channels',{}).get('github',{})
    if github.get('publish') not in (True,False): fail('channels.github.publish must be boolean')
    if github.get('packageKind')!='FunctionalPreview': fail('v0.4 packageKind must be FunctionalPreview')
    if github.get('platform')!='windows' or github.get('arch')!='x64': fail('v0.4 preview must target windows x64')
    print(f"release contract OK: ModernQGIS v{version}, package={github['packageKind']}, publish={github['publish']}"); return 0
if __name__=='__main__': raise SystemExit(main())
