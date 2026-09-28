"""Печатный листок из README.md и приложенных примеров (нужен reportlab)."""

from html import escape
from pathlib import Path
import re

from reportlab.lib import colors
from reportlab.lib.pagesizes import A4
from reportlab.lib.styles import ParagraphStyle
from reportlab.lib.units import mm
from reportlab.pdfbase import pdfmetrics
from reportlab.pdfbase.ttfonts import TTFont
from reportlab.platypus import (
    BaseDocTemplate, Flowable, Frame, PageBreak, PageTemplate,
    Paragraph, Preformatted, Spacer, Table, TableStyle,
)


HERE = Path(__file__).resolve().parent
OUTPUT = HERE.parent / "output" / "pdf" / "persistent-data-structures.pdf"
FONT_DIRS = [
    Path.home() / ".cache/codex-runtimes/codex-primary-runtime/dependencies"
    / "native/libreoffice-headless/libreoffice/LibreOfficeDev.app"
    / "Contents/Resources/fonts/truetype",
    Path("/usr/share/fonts/truetype/dejavu"),
]


def font_path(name):
    for directory in FONT_DIRS:
        path = directory / name
        if path.is_file():
            return str(path)
    raise FileNotFoundError(f"Не найден шрифт {name}; проверьте FONT_DIRS")


for name, filename in [
    ("Text", "DejaVuSerif.ttf"),
    ("Text-Bold", "DejaVuSerif-Bold.ttf"),
    ("Sans", "DejaVuSans.ttf"),
    ("Sans-Bold", "DejaVuSans-Bold.ttf"),
    ("Mono", "DejaVuSansMono.ttf"),
]:
    pdfmetrics.registerFont(TTFont(name, font_path(filename)))
pdfmetrics.registerFontFamily("Text", normal="Text", bold="Text-Bold")
pdfmetrics.registerFontFamily("Sans", normal="Sans", bold="Sans-Bold")

PAGE_W, PAGE_H = A4
MARGIN = 17 * mm
WIDTH = PAGE_W - 2 * MARGIN
INK = colors.HexColor("#181818")
MUTED = colors.HexColor("#555555")
RULE = colors.HexColor("#b5b5b5")

styles = {
    "body": ParagraphStyle(
        "body", fontName="Text", fontSize=10, leading=13.6,
        spaceAfter=5, textColor=INK, allowWidows=0, allowOrphans=0,
    ),
    "title": ParagraphStyle(
        "title", fontName="Sans-Bold", fontSize=19, leading=24,
        spaceBefore=0, spaceAfter=12, textColor=INK,
        keepWithNext=True,
    ),
    "h2": ParagraphStyle(
        "h2", fontName="Sans-Bold", fontSize=14, leading=18,
        spaceBefore=9, spaceAfter=8, keepWithNext=True,
    ),
    "h3": ParagraphStyle(
        "h3", fontName="Sans-Bold", fontSize=11, leading=14.5,
        spaceBefore=8, spaceAfter=6, keepWithNext=True,
    ),
    "label": ParagraphStyle(
        "label", fontName="Sans-Bold", fontSize=9, leading=12,
        spaceBefore=4, spaceAfter=4, keepWithNext=True,
    ),
    "code": ParagraphStyle(
        "code", fontName="Mono", fontSize=8.7, leading=10.6,
        spaceAfter=0,
    ),
    "small": ParagraphStyle(
        "small", fontName="Sans", fontSize=8, leading=10.5,
        textColor=MUTED,
    ),
}
styles["bullet"] = ParagraphStyle(
    "bullet", parent=styles["body"], leftIndent=11, firstLineIndent=-9,
    spaceAfter=3,
)


def normalize(text):
    return text.translate(str.maketrans({"—": "-", "–": "-", "‑": "-", "−": "-"}))


def inline(text):
    parts = re.split(r"(`[^`]+`)", normalize(text))
    result = []
    for part in parts:
        if part.startswith("`") and part.endswith("`"):
            result.append('<font name="Mono" size="8.8">' + escape(part[1:-1]) + "</font>")
        else:
            part = escape(part)
            part = re.sub(r"\*\*(.+?)\*\*", r"<b>\1</b>", part)
            result.append(part)
    return "".join(result)


def code_box(text, width):
    box = Table([[Preformatted(normalize(text), styles["code"])]], colWidths=[width])
    box.setStyle(TableStyle([
        ("BACKGROUND", (0, 0), (-1, -1), colors.HexColor("#f4f4f4")),
        ("LINEBEFORE", (0, 0), (0, -1), 1.1, RULE),
        ("LEFTPADDING", (0, 0), (-1, -1), 9),
        ("RIGHTPADDING", (0, 0), (-1, -1), 9),
        ("TOPPADDING", (0, 0), (-1, -1), 6),
        ("BOTTOMPADDING", (0, 0), (-1, -1), 6),
    ]))
    return [box, Spacer(1, 5)]


def markdown(text, width=WIDTH):
    """Небольшой разбор только используемых в листке элементов Markdown."""
    blocks = []
    for chunk in re.split(r"(```[^\n]*\n.*?\n```)", text.strip(), flags=re.S):
        if chunk.startswith("```"):
            blocks.extend(code_box("\n".join(chunk.splitlines()[1:-1]), width))
            continue
        for paragraph in re.split(r"\n\s*\n", chunk.strip()):
            if not paragraph.strip():
                continue
            line = " ".join(part.strip() for part in paragraph.splitlines())
            if line.startswith("Файлы:"):
                continue
            if line.startswith("Учебная цель"):
                # Служебная цель опускается, но вопросы студентам сохраняются.
                question = re.search(r"\b(Объясните|Докажите|Обязательно)\b", line)
                if question is None:
                    continue
                line = line[question.start():]
            if line.startswith("### "):
                blocks.append(Paragraph(inline(line[4:]), styles["h3"]))
            elif line.startswith("## "):
                line = line[3:].replace(" · домашнее задание", "")
                blocks.append(Paragraph(inline(line), styles["h2"]))
            elif line.startswith("# "):
                blocks.append(Paragraph(inline(line[2:]), styles["title"]))
            elif re.match(r"(?:- |\d+\. )", line):
                items = re.split(r"\n(?=(?:- |\d+\. ))", paragraph)
                for item in items:
                    item = " ".join(part.strip() for part in item.splitlines())
                    blocks.append(Paragraph(inline(item), styles["bullet"]))
            elif line in ("Формат ввода:", "Формат вывода:"):
                blocks.append(Paragraph(inline(line), styles["label"]))
            else:
                blocks.append(Paragraph(inline(line), styles["body"]))
    return blocks


def example(stem, width=WIDTH):
    data = (HERE / (stem + ".txt")).read_text().rstrip()
    expected = (HERE / (stem + ".expected.txt")).read_text().rstrip()
    table = Table([
        [Paragraph("Ввод", styles["label"]), Paragraph("Вывод", styles["label"])],
        [Preformatted(data, styles["code"]), Preformatted(expected, styles["code"])],
    ], colWidths=[width / 2, width / 2], hAlign="LEFT")
    table.setStyle(TableStyle([
        ("VALIGN", (0, 0), (-1, -1), "TOP"),
        ("LINEBELOW", (0, 0), (-1, 0), 0.5, RULE),
        ("LINEBEFORE", (1, 0), (1, -1), 0.5, RULE),
        ("BOX", (0, 0), (-1, -1), 0.5, RULE),
        ("LEFTPADDING", (0, 0), (-1, -1), 9),
        ("RIGHTPADDING", (0, 0), (-1, -1), 9),
        ("TOPPADDING", (0, 0), (-1, -1), 5),
        ("BOTTOMPADDING", (0, 0), (-1, -1), 5),
    ]))
    return [Paragraph("Пример", styles["label"]), table]


def task_with_example(text, stem):
    before, formats = text.split("Формат ввода:", 1)
    gap = 16
    left = (WIDTH - gap) * 0.52
    right = WIDTH - gap - left
    table = Table([[
        markdown("Формат ввода:" + formats, left),
        "",
        example(stem, right),
    ]], colWidths=[left, gap, right], hAlign="LEFT")
    table.setStyle(TableStyle([
        ("VALIGN", (0, 0), (-1, -1), "TOP"),
        ("LEFTPADDING", (0, 0), (-1, -1), 0),
        ("RIGHTPADDING", (0, 0), (-1, -1), 0),
        ("TOPPADDING", (0, 0), (-1, -1), 0),
        ("BOTTOMPADDING", (0, 0), (-1, -1), 0),
    ]))
    return markdown(before) + [table]


class Notes(Flowable):
    def __init__(self, height):
        super().__init__()
        self.width = WIDTH
        self.height = height

    def draw(self):
        self.canv.setFont("Sans", 8)
        self.canv.setFillColor(MUTED)
        self.canv.drawString(0, self.height - 12, "Место для схем и замечаний")
        self.canv.setStrokeColor(colors.HexColor("#d4d4d4"))
        self.canv.setLineWidth(0.35)
        self.canv.rect(0, 0, self.width, self.height - 24)


def on_page(canvas, doc):
    canvas.saveState()
    canvas.setTitle("Персистентные структуры данных - семинарский листок")
    canvas.setAuthor("Алгоритмы и структуры данных")
    canvas.setSubject("Задачи, упражнения для обсуждения и домашнее задание")
    canvas.setFillColor(MUTED)
    canvas.setFont("Sans", 7.6)
    canvas.drawString(MARGIN, PAGE_H - 12 * mm, "АЛГОРИТМЫ И СТРУКТУРЫ ДАННЫХ")
    canvas.drawRightString(PAGE_W - MARGIN, PAGE_H - 12 * mm, "Семинарский листок")
    canvas.setStrokeColor(RULE)
    canvas.setLineWidth(0.5)
    canvas.line(MARGIN, 14 * mm, PAGE_W - MARGIN, 14 * mm)
    canvas.drawString(MARGIN, 9 * mm, "Персистентные структуры данных")
    canvas.drawRightString(PAGE_W - MARGIN, 9 * mm, str(doc.page))
    canvas.restoreState()


def build():
    text = (HERE / "README.md").read_text()
    intro, rest = text.split("## 1. Стек на списке", 1)
    one, rest = ("## 1. Стек на списке" + rest).split("### На доске: история стека", 1)
    stack_board, rest = ("### На доске: история стека" + rest).split("## 2. Персистентный стек", 1)
    two, rest = ("## 2. Персистентный стек" + rest).split("## 3. Массив на дереве отрезков", 1)
    three, rest = ("## 3. Массив на дереве отрезков" + rest).split("### На доске: история массива", 1)
    array_board, rest = ("### На доске: история массива" + rest).split("## 4. Персистентный массив", 1)
    four, rest = ("## 4. Персистентный массив" + rest).split("### Обсуждение: почему с очередью сложнее?", 1)
    queue_board, rest = ("### Обсуждение: почему с очередью сложнее?" + rest).split("## 5. ", 1)
    five = ("## 5. " + rest).split("## Примеры и проверка", 1)[0]

    # Печатная версия сохраняет условия; организационные указания преподавателю
    # и перечни файлов заменены примерами непосредственно на страницах.
    intro = re.sub(
        r"Листок рассчитан.*?Нужны односвязные",
        "Задачи 1-4 выполняются на семинаре; задача 5 - домашняя.\n\nНужны односвязные",
        intro, flags=re.S,
    )
    intro = intro.replace("Эталонные решения написаны на C++17.", "Язык: C++17.")
    pages = [
        markdown(intro) + task_with_example(one, "01_stack"),
        markdown(stack_board) + task_with_example(two, "02_persistent_stack"),
        task_with_example(three, "03_array") + [Spacer(1, 14), Notes(100)],
        markdown(array_board) + task_with_example(four, "04_persistent_array"),
        markdown(queue_board) + [Spacer(1, 14), Notes(240)],
        [Paragraph("ДОМАШНЕЕ ЗАДАНИЕ", styles["label"])]
        + task_with_example(five, "05_persistent_queue"),
    ]
    story = []
    for i, page in enumerate(pages):
        if i:
            story.append(PageBreak())
        story.extend(page)
    OUTPUT.parent.mkdir(parents=True, exist_ok=True)
    doc = BaseDocTemplate(
        str(OUTPUT), pagesize=A4, leftMargin=MARGIN, rightMargin=MARGIN,
        topMargin=18 * mm, bottomMargin=19 * mm,
    )
    frame = Frame(
        MARGIN, 19 * mm, WIDTH, PAGE_H - 37 * mm,
        leftPadding=0, rightPadding=0, topPadding=0, bottomPadding=0,
    )
    doc.addPageTemplates(PageTemplate(id="sheet", frames=[frame], onPage=on_page))
    doc.build(story)
    print(OUTPUT)


if __name__ == "__main__":
    build()
