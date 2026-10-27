#!/usr/bin/env python3

"""Generates ThirdParty.txt shown in the About dialog from attribution pages of Qt documentation.

Usage: GenerateThirdParty.py <Qt documentation directory> [output file]

Qt documentation directory is the one installed by Qt Online Installer, e.g. ~/Qt/Docs/Qt-6.11.2.
See README.md next to this script for details.
"""

import glob
import html
import os
import re
import sys

# Qt modules the application is linked with (including the SVG icon engine plugin)
MODULES = ["qtcore", "qtgui", "qtnetwork", "qtdbus", "qtsvg"]

# Components that never get into deployed binaries: parts of the build system, other platforms,
# disabled features and plugins that are not deployed
EXCLUDED = {
    "android-gradle-wrapper",  # Android build system
    "extra-cmake-modules",  # Build system
    "kwin",  # Build system
    "qeventdispatcher-cf",  # macOS only
    "aglfn",  # PDF generator, disabled
    "android-native-style",  # Android only
    "cocoa-platform-plugin",  # macOS only
    "dejayvu",  # WebAssembly only
    "pixman",  # ARM NEON only
    "libjpeg",  # JPEG image plugin, not deployed
}


def strip_tags(fragment):
    return html.unescape(re.sub(r"<[^>]+>", "", fragment))


def dedent(block):
    # Qt documentation indents preformatted blocks with a single space
    lines = block.strip("\n").splitlines()
    lines = [line[1:] if line.startswith(" ") else line for line in lines]
    return "\n".join(line.rstrip() for line in lines).strip("\n")


def parse_attribution(path):
    page = open(path, encoding="utf-8").read()

    title = strip_tags(re.search(r'<h1 class="title">(.*?)</h1>', page, re.S).group(1)).strip()
    body = re.search(r'<div class="descr" id="details">(.*?)<!-- @@@', page, re.S).group(1)

    parts = []

    for match in re.finditer(r"<p>(.*?)</p>|<pre[^>]*>(.*?)</pre>", body, re.S):
        if match.group(1) is not None:
            paragraph = " ".join(strip_tags(match.group(1)).split())

            # Links and paths to Qt source tree make no sense in plain text
            if paragraph.startswith("Project Homepage") or paragraph.startswith("The sources can be found"):
                continue

            parts.append(paragraph)
        else:
            parts.append(dedent(strip_tags(match.group(2))))

    return f"{title}\n{'=' * len(title)}\n\n" + "\n\n".join(parts)


def main():
    if len(sys.argv) not in (2, 3):
        print(__doc__.strip(), file=sys.stderr)
        return 1

    docs_dir = sys.argv[1]
    output_path = sys.argv[2] if len(sys.argv) == 3 else os.path.join(os.path.dirname(__file__), "ThirdParty.txt")

    sections = []
    found_keys = set()

    for module in MODULES:
        paths = sorted(glob.glob(os.path.join(docs_dir, module, f"{module}-attribution-*.html")))

        if not paths:
            print(f"No attribution pages found for {module} in {docs_dir}", file=sys.stderr)
            return 1

        for path in paths:
            key = path.rsplit("attribution-", 1)[1][:-len(".html")]
            found_keys.add(key)

            if key not in EXCLUDED:
                sections.append(parse_attribution(path))

    for key in sorted(EXCLUDED - found_keys):
        print(f"Warning: excluded component {key} is not present anymore", file=sys.stderr)

    with open(output_path, "w", encoding="utf-8", newline="\n") as file:
        file.write("\n\n\n".join(sections) + "\n")

    print(f"{len(sections)} components written to {output_path}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
