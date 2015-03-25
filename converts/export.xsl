<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output method="text" encoding="iso-8859-15" indent="no"/>
<xsl:template match="//font[@color='#ff0000']">SEND
<xsl:value-of select="."/>
END
</xsl:template>
<xsl:template match="//font[@color='#0000ff']">EXPECT
<xsl:value-of select="."/>
END
</xsl:template>
<xsl:template match="text()|@*"></xsl:template>
</xsl:stylesheet>
