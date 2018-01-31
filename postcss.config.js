
module.exports = {
	parser: require('postcss-scss'), // For inline comment support in .pcss files.
	plugins: [
		require('postcss-import'),
		require('postcss-mixins'),
		require('postcss-property-lookup'),
		require('postcss-nested'),
		require('postcss-simple-vars'),
		require('postcss-color-function'),
		require('autoprefixer'),
	]
}