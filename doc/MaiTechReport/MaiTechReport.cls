% =============================================================================
%  MaiTechReport.cls
%  2026 江苏省大学生机器人大赛技术报告版式类
%  依据：2023江苏省大学生机器人大赛技术报告撰写规范
%
%  版式硬指标（逐条对应规范）：
%    A4 单面；页边距上下左右 2cm；装订线左 0.5cm
%    正文宋体小四（12pt），固定行距 18pt
%    页眉宋体小五（9pt）居中：「江苏省大学生机器人大赛技术报告」
%    页码宋体小五居右：「第X页」；页眉距边界 1.5cm，页脚距边界 1.75cm
%    一级标题：小三（15pt）黑体、居中、编号后空一格
%    二级标题：四号（14pt）黑体、顶格
%    三级标题：小四（12pt）宋体、加粗、顶格
% =============================================================================
\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{MaiTechReport}[2026/10/25 v1.0 Mai competition technical report]

% 中文支持：ctex + Windows 字体集（宋体/黑体/楷体/仿宋）
% 字体已用户级安装到 ~/.local/share/fonts/MaiFonts/
\LoadClass[12pt,a4paper,fontset=windows]{ctexart}

% -----------------------------------------------------------------------------
% 页面：A4，四边 2cm，装订线 0.5cm
% headheight/headsep 让页眉顶端落在距纸边 1.5cm 处：
%   2.0cm - 0.09cm - 0.46cm ≈ 1.45cm ≈ 1.5cm
% -----------------------------------------------------------------------------
\RequirePackage[
  a4paper,
  top=2cm, bottom=2cm,
  left=2cm, right=2cm,
  bindingoffset=0.5cm,
  headheight=13pt, headsep=2.5pt,
  % 规范：页脚距边界 1.75cm。geometry 的 footskip 是「正文底边到页脚基线」，
  % 与 Word 的「页脚距边界」不是同一个量；按 9pt 页脚折算约 0.55cm。
  % 本报告页码在页眉，页脚为空，此设置只保证以后加页脚时位置正确。
  footskip=0.55cm
]{geometry}

% -----------------------------------------------------------------------------
% 字体与行距
% -----------------------------------------------------------------------------
% 宋体的「加粗」在 Word 里是伪粗体；ctex 的 windows 字体集默认把 \textbf
% 映射成黑体。这里单独定义一个「宋体伪粗」家族，保证三级标题仍是宋体。
\setCJKfamilyfont{zhsongbf}{SimSun}[AutoFakeBold=2.5]
\newcommand{\songbf}{\CJKfamily{zhsongbf}\bfseries}

% 正文：宋体小四 12pt，固定行距 18pt
\renewcommand{\baselinestretch}{1.0}
\AtBeginDocument{%
  \fontsize{12pt}{18pt}\selectfont
  \setlength{\parindent}{2em}%
  \setlength{\parskip}{0pt}%
}
\setlength{\emergencystretch}{1em}

% 西文正文与代码字体
\setmainfont{Times New Roman}
\setsansfont{Liberation Sans}[BoldFont=Liberation Sans Bold]
\setmonofont{DejaVu Sans Mono}[Scale=0.92]
\setCJKmonofont{SimSun}

% -----------------------------------------------------------------------------
% 页眉页脚
% -----------------------------------------------------------------------------
\RequirePackage{fancyhdr}
\pagestyle{fancy}
\fancyhf{}
\fancyhead[C]{\songti\fontsize{9pt}{11pt}\selectfont 江苏省大学生机器人大赛技术报告}
\fancyhead[R]{\songti\fontsize{9pt}{11pt}\selectfont 第\mbox{\thepage}页}
\renewcommand{\headrulewidth}{0pt}
\renewcommand{\footrulewidth}{0pt}
% 章节首页也用同一页眉（规范要求整篇统一）
\fancypagestyle{plain}{%
  \fancyhf{}%
  \fancyhead[C]{\songti\fontsize{9pt}{11pt}\selectfont 江苏省大学生机器人大赛技术报告}%
  \fancyhead[R]{\songti\fontsize{9pt}{11pt}\selectfont 第\mbox{\thepage}页}%
  \renewcommand{\headrulewidth}{0pt}%
}

% -----------------------------------------------------------------------------
% 标题层级
% -----------------------------------------------------------------------------
\ctexset{
  section = {
    name     = {,},
    number   = \arabic{section},
    aftername = \hspace{1em},
    format   = \zihao{-3}\heiti\centering,
    indent   = 0pt,
    beforeskip = 9pt plus 2pt minus 2pt,
    afterskip  = 8pt plus 2pt minus 2pt,
  },
  subsection = {
    name     = {,},
    number   = \arabic{section}.\arabic{subsection},
    aftername = \hspace{0.5em},
    format   = \zihao{4}\heiti\raggedright,
    indent   = 0pt,
    beforeskip = 7pt plus 2pt minus 2pt,
    afterskip  = 4pt plus 2pt minus 2pt,
  },
  subsubsection = {
    name     = {,},
    number   = \arabic{section}.\arabic{subsection}.\arabic{subsubsection},
    aftername = \hspace{0.5em},
    format   = \zihao{-4}\songbf\raggedright,
    indent   = 0pt,
    beforeskip = 6pt plus 2pt minus 2pt,
    afterskip  = 3pt plus 2pt minus 2pt,
  },
}

% -----------------------------------------------------------------------------
% 目录：三号黑体居中，「目」与「录」之间空四格，三级目录
% -----------------------------------------------------------------------------
\RequirePackage{tocloft}
\setcounter{tocdepth}{3}
\renewcommand{\contentsname}{目\hspace{4em}录}
\renewcommand{\cfttoctitlefont}{\hfill\zihao{3}\heiti}
\renewcommand{\cftaftertoctitle}{\hfill}
\setlength{\cftbeforetoctitleskip}{0pt}
\setlength{\cftaftertoctitleskip}{18pt}
\renewcommand{\cftsecfont}{\songti\zihao{-4}}
\renewcommand{\cftsecpagefont}{\songti\zihao{-4}}
\renewcommand{\cftsubsecfont}{\songti\zihao{-4}}
\renewcommand{\cftsubsecpagefont}{\songti\zihao{-4}}
\renewcommand{\cftsubsubsecfont}{\songti\zihao{-4}}
\renewcommand{\cftsubsubsecpagefont}{\songti\zihao{-4}}
\setlength{\cftsecindent}{0pt}
\setlength{\cftsubsecindent}{2em}
\setlength{\cftsubsubsecindent}{4em}
\renewcommand{\cftsecleader}{\cftdotfill{\cftdotsep}}
\renewcommand{\cftsubsecleader}{\cftdotfill{\cftdotsep}}
\renewcommand{\cftsubsubsecleader}{\cftdotfill{\cftdotsep}}

% -----------------------------------------------------------------------------
% 图表、公式
% -----------------------------------------------------------------------------
\RequirePackage{graphicx}
\RequirePackage{tikz}
\usetikzlibrary{arrows.meta,positioning,shapes.geometric,fit,calc,backgrounds}
\RequirePackage{booktabs}
\RequirePackage{array}
\RequirePackage{tabularx}
\RequirePackage{longtable}
\RequirePackage{multirow}
\RequirePackage{makecell}
\RequirePackage{amsmath,amssymb}
\RequirePackage{caption}
\DeclareCaptionFont{wuhao}{\zihao{5}}
\DeclareCaptionFont{maiSong}{\songti}
\captionsetup{
  font={maiSong,wuhao},
  labelsep=quad,
  justification=centering,
  skip=6pt,
}
\captionsetup[table]{position=top}
\captionsetup[figure]{position=bottom}
\renewcommand{\figurename}{图}
\renewcommand{\tablename}{表}
\numberwithin{figure}{section}
\numberwithin{table}{section}
\numberwithin{equation}{section}

\RequirePackage{float}
\renewcommand{\topfraction}{0.85}
\renewcommand{\bottomfraction}{0.75}
\renewcommand{\textfraction}{0.12}
\renewcommand{\floatpagefraction}{0.8}
\setlength{\textfloatsep}{8pt plus 2pt minus 2pt}
\setlength{\floatsep}{8pt plus 2pt minus 2pt}
\setlength{\intextsep}{8pt plus 2pt minus 2pt}
\renewcommand{\arraystretch}{1.1}
\setlength{\tabcolsep}{4pt}
% 表格正文用五号宋体（规范未单列，按学位论文惯例）
\AtBeginEnvironment{table}{\zihao{5}\songti}
\AtBeginEnvironment{longtable}{\zihao{5}\songti}

% -----------------------------------------------------------------------------
% 列表
% -----------------------------------------------------------------------------
\RequirePackage{etoolbox}
\RequirePackage{enumitem}
\setlist[itemize]{leftmargin=2em,itemsep=0pt,topsep=3pt,parsep=0pt}
\setlist[enumerate]{leftmargin=2.2em,itemsep=0pt,topsep=3pt,parsep=0pt}

% -----------------------------------------------------------------------------
% 代码清单（附录用）
% -----------------------------------------------------------------------------
\RequirePackage{listings}
\RequirePackage{xcolor}
\definecolor{MaiCodeGray}{gray}{0.95}
\lstset{
  basicstyle=\ttfamily\zihao{5},
  keywordstyle=\bfseries,
  commentstyle=\color{gray},
  stringstyle=\color{black},
  numbers=left,
  numberstyle=\tiny\color{gray},
  numbersep=6pt,
  frame=single,
  rulecolor=\color{gray},
  backgroundcolor=\color{MaiCodeGray},
  breaklines=true,
  breakatwhitespace=false,
  columns=fullflexible,
  keepspaces=true,
  showstringspaces=false,
  tabsize=4,
  extendedchars=true,
  xleftmargin=1.2em,
  xrightmargin=1em,
}

% -----------------------------------------------------------------------------
% 超链接（不显示边框，仅用于 PDF 书签）
% -----------------------------------------------------------------------------
\RequirePackage[hidelinks,bookmarksnumbered=true]{hyperref}

% -----------------------------------------------------------------------------
% 正文层级中的「1）」「①」等，用现成命令即可；这里只约定段内编号缩进
% -----------------------------------------------------------------------------
\newcommand{\MaiLevelOne}[1]{\par\noindent\hspace{2em}#1}
\newcommand{\MaiLevelTwo}[1]{\par\noindent\hspace{2em}#1}

% -----------------------------------------------------------------------------
% 封面
%   参数：#1 赛项  #2 学校  #3 队伍  #4 队员  #5 教师  #6 日期
% -----------------------------------------------------------------------------
\newcommand{\MaiCover}[6]{%
  \begin{titlepage}
  \thispagestyle{empty}%
  \centering
  \vspace*{2.0cm}
  {\heiti\zihao{-2} 江苏省大学生机器人大赛\par}
  \vspace{2.2cm}
  {\heiti\zihao{2} #1\par}
  \vspace{3.0cm}
  {\heiti\zihao{2} 技术报告\par}
  \vfill
  {\songti\zihao{-3}
   \renewcommand{\arraystretch}{2.0}%
   \begin{tabular}{@{}r@{\hspace{0.3em}}c@{}}
     参赛学校： & \underline{\makebox[7.2cm][c]{#2}} \\
     队伍名称： & \underline{\makebox[7.2cm][c]{#3}} \\
     参赛队员： & \underline{\makebox[7.2cm][c]{#4}} \\
     带队教师： & \underline{\makebox[7.2cm][c]{#5}} \\
   \end{tabular}\par}
  \vspace{2.0cm}
  {\songti\zihao{-3} 日期：\underline{\makebox[4.6cm][c]{#6}}\par}
  \vspace{1.2cm}
  \end{titlepage}
}

% -----------------------------------------------------------------------------
% 技术报告使用授权说明
%   若 MaiFigs/Mai授权页扫描.pdf 或 .jpg 存在，整页插图（签名页扫描件）；
%   否则排版出空白签名栏，供打印后手写签字再扫描替换。
% -----------------------------------------------------------------------------
\newcommand{\MaiAuthorization}{%
  \clearpage
  \thispagestyle{empty}%
  \IfFileExists{MaiFigs/Mai授权页扫描.pdf}{%
    \noindent\includegraphics[width=\paperwidth,height=\paperheight]{MaiFigs/Mai授权页扫描.pdf}%
  }{%
    \IfFileExists{MaiFigs/Mai授权页扫描.jpg}{%
      \noindent\includegraphics[width=\paperwidth,height=\paperheight]{MaiFigs/Mai授权页扫描.jpg}%
    }{%
      \begin{center}
      \vspace*{1.2cm}
      {\heiti\zihao{2} 关于技术报告使用授权的说明\par}
      \end{center}
      \vspace{1.2cm}
      {\songti\zihao{-4}
      本人完全了解 2026 江苏省大学生机器人大赛关于保留、使用技术报告和研究论文的规定，即：参赛作品著作权归参赛者本人和比赛组委会共同所有，比赛组委会可以在相关主页上收录并公开参赛作品的设计方案、技术报告以及参赛机器人的视频、图像资料，并将相关内容编纂收录在组委会出版论文集中。\par}
      \vspace{4.5cm}
      \begin{flushright}
      \songti\zihao{5}
      \renewcommand{\arraystretch}{2.0}%
      \begin{tabular}{@{}l@{}}
        参赛队员签名：\underline{\makebox[4.5cm][l]{}} \\
        带队教师签名：\underline{\makebox[4.5cm][l]{}} \\
        日\hspace{2em}期：\underline{\makebox[4.5cm][l]{}} \\
      \end{tabular}
      \end{flushright}
    }%
  }%
}

% -----------------------------------------------------------------------------
% 中文摘要
%   参数：#1 标题  #2 作者/单位  #3 摘要正文  #4 关键词
% -----------------------------------------------------------------------------
\newcommand{\MaiChineseAbstract}[4]{%
  \clearpage
  \pagenumbering{arabic}%
  \thispagestyle{fancy}%
  \begin{center}
    {\heiti\zihao{3} #1\par}
    \vspace{6pt}
    {\songti\zihao{-4} #2\par}
  \end{center}
  \vspace{12pt}
  \begin{center}
    {\heiti\zihao{3} 摘\hspace{1em}要\par}
  \end{center}
  \vspace{6pt}
  {\songti\zihao{-4} #3\par}
  \vspace{12pt}
  \noindent{\songti\zihao{-4}\textbf{关键词：}#4\par}
  \clearpage
}

% -----------------------------------------------------------------------------
% 英文摘要
%   参数：#1 英文标题  #2 作者/单位  #3 摘要正文  #4 关键词
% -----------------------------------------------------------------------------
\newcommand{\MaiEnglishAbstract}[4]{%
  \thispagestyle{fancy}%
  \begin{center}
    {\bfseries\fontsize{16pt}{20pt}\selectfont #1\par}
    \vspace{6pt}
    {\fontsize{12pt}{18pt}\selectfont #2\par}
  \end{center}
  \vspace{12pt}
  \begin{center}
    {\bfseries\fontsize{16pt}{20pt}\selectfont ABSTRACT\par}
  \end{center}
  \vspace{6pt}
  {\fontsize{12pt}{18pt}\selectfont #3\par}
  \vspace{12pt}
  \noindent{\fontsize{12pt}{18pt}\selectfont\textbf{Key words:} #4\par}
  \clearpage
}

% -----------------------------------------------------------------------------
% 目录
% -----------------------------------------------------------------------------
\newcommand{\MaiTableOfContents}{%
  \clearpage
  \thispagestyle{fancy}%
  \tableofcontents
  \clearpage
}

% -----------------------------------------------------------------------------
% 参考文献：标题四号黑体顶格；条目五号宋体/ Times New Roman
%   用标准 thebibliography，保证 \cite 编号与条目一致；
%   只在环境内把 section 格式改成「四号黑体顶格」。
% -----------------------------------------------------------------------------
\newenvironment{MaiReferences}{%
  \clearpage
  \phantomsection
  \addcontentsline{toc}{section}{参考文献}%
  \begingroup
  \ctexset{section/format={\zihao{4}\heiti\raggedright}}%
  \begin{thebibliography}{99}%
  \zihao{5}\songti
  \setlength{\labelwidth}{2.2em}%
  \setlength{\leftmargin}{2.7em}%
  \setlength{\itemsep}{0pt}%
  \setlength{\parsep}{0pt}%
}{%
  \end{thebibliography}%
  \endgroup
}
% 正文引用采用上标 [x]
\newcommand{\upcite}[1]{\textsuperscript{\cite{#1}}}

% -----------------------------------------------------------------------------
% 致谢：二字间空两格、四号黑体居中；正文小四宋体
% -----------------------------------------------------------------------------
\newcommand{\MaiAcknowledgments}[1]{%
  \clearpage
  \phantomsection
  \addcontentsline{toc}{section}{致谢}%
  \begin{center}
    {\heiti\zihao{4} 致\hspace{2em}谢\par}
  \end{center}
  \vspace{6pt}
  {\songti\zihao{-4} #1\par}
}

% -----------------------------------------------------------------------------
% 附录标题
% -----------------------------------------------------------------------------
\newcommand{\MaiAppendix}[1]{%
  \clearpage
  \phantomsection
  \section*{#1}%
  \addcontentsline{toc}{section}{#1}%
}

\endinput
