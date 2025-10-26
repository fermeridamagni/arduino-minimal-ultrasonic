import { defineConfig } from "vitepress";

// https://vitepress.dev/reference/site-config
export default defineConfig({
	title: "MinimalUltrasonic",
	description:
		"A minimalist, efficient Arduino library for ultrasonic distance sensors with multiple unit support",
	base: "/arduino-minimal-ultrasonic/",

	themeConfig: {
		// https://vitepress.dev/reference/default-theme-config
		logo: "/logo.svg",

		nav: [
			{ text: "Home", link: "/" },
			{ text: "Guide", link: "/guide/getting-started" },
			{ text: "API", link: "/api/reference" },
			{ text: "Examples", link: "/examples/basic" },
			{ text: "Technical", link: "/technical/architecture" },
			{
				text: "v2.0.0",
				items: [
					{ text: "Changelog", link: "/changelog" },
					{ text: "Migration Guide", link: "/guide/migration" },
				],
			},
		],

		sidebar: {
			"/guide/": [
				{
					text: "Introduction",
					items: [
						{
							text: "What is MinimalUltrasonic?",
							link: "/guide/what-is-minimalultrasonic",
						},
						{ text: "Getting Started", link: "/guide/getting-started" },
						{ text: "Installation", link: "/guide/installation" },
					],
				},
				{
					text: "Usage",
					items: [
						{ text: "Basic Usage", link: "/guide/basic-usage" },
						{ text: "Multiple Units", link: "/guide/multiple-units" },
						{
							text: "Timeout Configuration",
							link: "/guide/timeout-configuration",
						},
						{ text: "Multiple Sensors", link: "/guide/multiple-sensors" },
						{ text: "Error Handling", link: "/guide/error-handling" },
					],
				},
				{
					text: "Advanced",
					items: [
						{ text: "Best Practices", link: "/guide/best-practices" },
						{ text: "Optimization", link: "/guide/optimization" },
						{ text: "Troubleshooting", link: "/guide/troubleshooting" },
					],
				},
				{
					text: "Migration",
					items: [{ text: "From v1.x to v2.0", link: "/guide/migration" }],
				},
			],
			"/api/": [
				{
					text: "API Reference",
					items: [
						{ text: "Overview", link: "/api/reference" },
						{ text: "MinimalUltrasonic Class", link: "/api/class" },
						{ text: "Constructors", link: "/api/constructors" },
						{ text: "Methods", link: "/api/methods" },
						{ text: "Unit Enum", link: "/api/units" },
						{ text: "Constants", link: "/api/constants" },
					],
				},
			],
			"/examples/": [
				{
					text: "Examples",
					items: [
						{ text: "Basic Example", link: "/examples/basic" },
						{ text: "All Units", link: "/examples/all-units" },
						{ text: "Timeout Configuration", link: "/examples/timeout" },
						{ text: "Multiple Sensors", link: "/examples/multiple-sensors" },
						{ text: "Advanced Usage", link: "/examples/advanced" },
					],
				},
			],
			"/technical/": [
				{
					text: "Technical Documentation",
					items: [
						{ text: "Architecture", link: "/technical/architecture" },
						{ text: "Physics Background", link: "/technical/physics" },
						{ text: "Unit Conversions", link: "/technical/conversions" },
						{ text: "Performance", link: "/technical/performance" },
						{ text: "Compatibility", link: "/technical/compatibility" },
					],
				},
			],
		},

		socialLinks: [
			{
				icon: "github",
				link: "https://github.com/fermeridamagni/arduino-minimal-ultrasonic",
			},
		],

		footer: {
			message: "Released under the MIT License.",
			copyright: "Copyright © 2025 fermeridamagni (Magni Development)",
		},

		search: {
			provider: "local",
		},

		editLink: {
			pattern:
				"https://github.com/fermeridamagni/arduino-minimal-ultrasonic/edit/master/docs/:path",
			text: "Edit this page on GitHub",
		},
	},

	markdown: {
		lineNumbers: true,
		theme: {
			light: "github-light",
			dark: "github-dark",
		},
	},
});
