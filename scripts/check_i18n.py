#!/usr/bin/env python3
from pathlib import Path
import sys, xml.etree.ElementTree as ET
ROOT=Path(__file__).resolve().parents[1]
FILES=[ROOT/'translations/modernqgis_zh_CN.ts',ROOT/'translations/modernqgis_ja_JP.ts',ROOT/'translations/modernqgis_en_US.ts']
def load(path):
    result={}
    for context in ET.parse(path).getroot().findall('context'):
        name=context.findtext('name',default='').strip()
        for message in context.findall('message'):
            source=message.findtext('source',default='').strip(); node=message.find('translation'); translation='' if node is None else ''.join(node.itertext()).strip()
            if not name or not source: raise ValueError(f'{path}: blank context/source')
            key=(name,source)
            if key in result: raise ValueError(f'{path}: duplicate {key}')
            if not translation or (node is not None and node.get('type')=='unfinished'): raise ValueError(f'{path}: incomplete {key}')
            result[key]=translation
    return result
def main():
    catalogs={p.name:load(p) for p in FILES}; base_name,base=next(iter(catalogs.items())); keys=set(base); errors=[]
    for name,catalog in catalogs.items():
        missing=sorted(keys-set(catalog)); extra=sorted(set(catalog)-keys)
        if missing: errors.append(f'{name}: missing {missing}')
        if extra: errors.append(f'{name}: extra {extra}')
    if errors: print('i18n contract failed:\n'+'\n'.join(errors),file=sys.stderr); return 1
    print(f'i18n contract OK: {len(keys)} keys across {len(catalogs)} locales ({base_name} baseline)'); return 0
if __name__=='__main__': raise SystemExit(main())
