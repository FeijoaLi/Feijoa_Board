import os
import sys
import pdfkit
import urllib.parse
from collections import defaultdict
from pygments import highlight
from pygments.lexers import CppLexer
from pygments.formatters import HtmlFormatter

def collect_cpp_files(root_dir):
    files = []
    for root, dirs, filenames in os.walk(root_dir):
        for filename in filenames:
            if filename.endswith('.cpp'):
                full_path = os.path.join(root, filename)
                rel_path = os.path.relpath(full_path, root_dir)
                files.append((rel_path, full_path))
    files.sort(key=lambda x: x[0])
    return files

def group_by_folder(files):
    groups = defaultdict(list)
    for rel_path, full_path in files:
        folder = os.path.dirname(rel_path)
        if not folder:
            folder = "根目录"
        groups[folder].append((rel_path, full_path))
    for folder in groups:
        groups[folder].sort(key=lambda x: x[0])
    return sorted(groups.items())

def read_file_content(path):
    encodings = ['utf-8', 'gbk', 'gb2312', 'utf-16']
    for enc in encodings:
        try:
            with open(path, 'r', encoding=enc) as f:
                return f.read()
        except (UnicodeDecodeError, LookupError):
            continue
    with open(path, 'r', encoding='utf-8', errors='ignore') as f:
        return f.read()

def generate_html(files):
    groups = group_by_folder(files)

    # Pygments 样式
    formatter = HtmlFormatter(style='vs', linenos=False, cssclass='codehilite')
    pygments_css = formatter.get_style_defs('.codehilite')

    html_parts = []
    html_parts.append(f'''<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<style>
    body {{
        font-family: Consolas, 'Microsoft YaHei', monospace;
        background-color: #f5f5f5;
        margin: 0;
        padding: 20px;
    }}
    .cover {{
        text-align: center;
        margin-top: 30%;
        font-size: 24px;
        page-break-after: always;
    }}
    .cover h1 {{
        font-size: 48px;
        margin-bottom: 20px;
    }}
    .cover p {{
        font-size: 18px;
        color: #555;
    }}
    h1 {{
        font-size: 28px;
        color: #2c3e50;
        border-bottom: 2px solid #3498db;
        padding-bottom: 10px;
        page-break-before: always;
        page-break-after: avoid;
    }}
    .file {{
        page-break-before: always;
        background-color: white;
        border: 1px solid #ccc;
        margin: 20px 0;
        padding: 10px;
        page-break-inside: avoid;
    }}
    .file h2 {{
        margin: 0 0 10px 0;
        font-size: 20px;
        color: #2980b9;
        border-bottom: 1px solid #bdc3c7;
        padding-bottom: 5px;
    }}
    pre {{
        margin: 0;
        font-family: Consolas, 'Microsoft YaHei', monospace;
        font-size: 12px;
        white-space: pre-wrap;
        word-wrap: break-word;
        background-color: #fafafa;
        padding: 10px;
        border-radius: 5px;
    }}
    code {{
        background-color: transparent;
    }}
    /* Pygments 生成的样式 */
    {pygments_css}
</style>
</head>
<body>
''')

    # 封面
    html_parts.append('''
<div class="cover">
    <h1>算法板子合集</h1>
    <p>代码模板 / 算法笔记</p>
    <p>生成日期：2026年3月</p>
    <p>包含语法高亮，按文件夹分类</p>
</div>
''')

    # 文件夹内容
    for folder, file_list in groups:
        folder_name = folder if folder != "根目录" else "📁 根目录"
        html_parts.append(f'<h1>{folder_name}</h1>')
        for rel_path, full_path in file_list:
            anchor_id = f"file-{urllib.parse.quote(rel_path, safe='')}"
            html_parts.append(f'<div class="file">')
            html_parts.append(f'<h2 id="{anchor_id}">{rel_path}</h2>')
            # 使用 Pygments 高亮代码
            content = read_file_content(full_path)
            highlighted = highlight(content, CppLexer(), formatter)
            html_parts.append(highlighted)
            html_parts.append(f'</div>')

    html_parts.append('</body></html>')
    return '\n'.join(html_parts)

def main():
    root_dir = os.path.dirname(os.path.abspath(__file__))
    if len(sys.argv) > 1:
        root_dir = sys.argv[1]

    files = collect_cpp_files(root_dir)
    if not files:
        print("未找到任何 .cpp 文件。")
        return

    html = generate_html(files)
    output_pdf = os.path.join(root_dir, 'Board.pdf')

    wkhtml_path = r'E:\Tool\Code\Wkhtmltox_Tools\wkhtmltopdf\bin\wkhtmltopdf.exe'
    config = pdfkit.configuration(wkhtmltopdf=wkhtml_path)

    options = {
        'encoding': 'UTF-8',
        'quiet': '',
        'enable-local-file-access': None,
        'margin-top': '15mm',
        'margin-bottom': '15mm',
        'margin-left': '10mm',
        'margin-right': '10mm',
        '--keep-relative-links': None,
    }

    # 使用空字典启用默认目录（会包含所有 h1, h2 标题）
    toc = {}

    try:
        pdfkit.from_string(html, output_pdf, options=options, toc=toc, configuration=config)
        print(f"✅ PDF 生成成功：{output_pdf}")
    except Exception as e:
        print("❌ 生成 PDF 时发生异常：", e)
        html_output = os.path.join(root_dir, 'Board.html')
        with open(html_output, 'w', encoding='utf-8') as f:
            f.write(html)
        print(f"📄 HTML 文件已生成：{html_output}")

if __name__ == '__main__':
    main()