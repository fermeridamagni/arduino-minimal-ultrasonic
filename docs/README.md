# MinimalUltrasonic Documentation

Comprehensive documentation for the MinimalUltrasonic Arduino library, built with [VitePress](https://vitepress.dev/).

## 📚 Documentation Structure

```
docs/
├── .vitepress/
│   └── config.mts          # VitePress configuration
├── guide/                  # User guides
│   ├── what-is-minimalultrasonic.md
│   ├── getting-started.md
│   ├── installation.md
│   ├── basic-usage.md
│   ├── units.md
│   ├── timeout.md
│   ├── multiple-sensors.md
│   ├── error-handling.md
│   ├── best-practices.md
│   ├── optimization.md
│   ├── troubleshooting.md
│   └── migration.md
├── api/                    # API reference
│   ├── reference.md
│   ├── class.md
│   ├── constructors.md
│   ├── methods.md
│   ├── units.md
│   └── constants.md
├── examples/               # Code examples
│   ├── basic.md
│   ├── all-units.md
│   ├── timeout.md
│   ├── multiple-sensors.md
│   └── advanced.md
├── technical/              # Technical documentation
│   ├── architecture.md
│   ├── physics.md
│   ├── conversions.md
│   ├── performance.md
│   └── compatibility.md
├── index.md               # Home page
├── changelog.md           # Version history
└── package.json           # Node dependencies

```

## 🚀 Getting Started

### Prerequisites

- Node.js 18 or higher
- npm or yarn

### Installation

1. **Navigate to docs directory**

   ```bash
   cd docs
   ```

2. **Install dependencies**

   ```bash
   npm install
   ```

### Development

Run the development server:

```bash
npm run docs:dev
```

The site will be available at `http://localhost:5173`

### Building

Build the static site:

```bash
npm run docs:build
```

Output will be in `docs/.vitepress/dist`

### Preview Build

Preview the built site:

```bash
npm run docs:preview
```

## 📝 Writing Documentation

### Adding a New Page

1. Create a new `.md` file in the appropriate directory
2. Add frontmatter if needed:

   ```md
   ---
   title: Page Title
   description: Page description
   ---
   ```

3. Update `docs/.vitepress/config.mts` to add to navigation/sidebar

### Markdown Features

VitePress supports extended Markdown:

#### Custom Containers

```md
::: tip
This is a tip
:::

::: warning
This is a warning
:::

::: danger
This is a danger warning
:::

::: info
This is an info box
:::
```

#### Code Blocks

````md
```cpp
#include <MinimalUltrasonic.h>

MinimalUltrasonic sensor(12, 13);
```
````

With line highlighting:

````md
```cpp{2,4-6}
#include <MinimalUltrasonic.h>

MinimalUltrasonic sensor(12, 13);

void setup() {
  Serial.begin(9600);
}
```
````

#### Tables

```md
| Feature | Description |
|---------|-------------|
| Units | 6 supported |
| Memory | 8 bytes |
```

## 🎨 Configuration

Main configuration is in `docs/.vitepress/config.mts`:

- **Site metadata** - title, description, base URL
- **Theme config** - navigation, sidebar, footer
- **Search** - local search enabled
- **Social links** - GitHub link
- **Edit links** - "Edit this page" functionality

## 📂 Directory Guide

### `/guide`

User-facing guides and tutorials. Start here for learning the library.

### `/api`

Complete API reference documentation. Technical specifications for all classes and methods.

### `/examples`

Working code examples with explanations. Copy-paste ready code.

### `/technical`

Deep technical documentation. Architecture, physics, performance details.

## 🔗 Links

- **Live Docs**: <https://fermeridamagni.github.io/arduino-minimal-ultrasonic/>
- **GitHub**: <https://github.com/fermeridamagni/arduino-minimal-ultrasonic>
- **VitePress**: <https://vitepress.dev/>

## 📋 Checklist for New Documentation

When adding new features to the library:

- [ ] Update relevant guide pages
- [ ] Add/update API reference
- [ ] Create example if needed
- [ ] Update changelog
- [ ] Test all code examples
- [ ] Build and preview locally
- [ ] Check for broken links

## 🤝 Contributing

To contribute to documentation:

1. Fork the repository
2. Create a branch (`git checkout -b docs/new-feature`)
3. Make your changes
4. Test locally (`npm run docs:dev`)
5. Commit (`git commit -am 'docs: add new feature guide'`)
6. Push (`git push origin docs/new-feature`)
7. Create a Pull Request

## 📄 License

Documentation is part of the MinimalUltrasonic project and released under the MIT License.

---

**Built with ❤️ using VitePress**
