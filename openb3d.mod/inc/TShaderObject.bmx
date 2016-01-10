Rem
bbdoc: ShaderObject
End Rem
Type TShaderObject
	Field instance:Byte Ptr
	
	Function CreateObject:TShaderObject( inst:Byte Ptr )
		
		If inst=Null Then Return Null
		Local obj:TShaderObject=New TShaderObject
		obj.instance=inst
		Return obj
		
	End Function
	
	Function GetInstance:Byte Ptr( obj:TShaderObject )
		If obj=Null Then Return Null
		Return obj.Instance
	End Function
	
	Function LoadShaderObject:TShaderObject( shadertype:Int,filename:String )
		Local name:Byte Ptr=filename.ToCString()
		Local inst:Byte Ptr=LoadShaderObject_( shadertype,name )
		Local obj:TShaderObject=CreateObject( inst )
		MemFree name
		Return obj
	End Function
	
	Function CreateShaderObject:TShaderObject( shadertype:Int,source:String,name:String )
		Local src:Byte Ptr=source.ToCString()
		Local nam:Byte Ptr=name.ToCString()
		Local inst:Byte Ptr=CreateShaderObject_( shadertype,src,nam )
		Local obj:TShaderObject=CreateObject( inst )
		MemFree src
		MemFree nam
		Return obj
	End Function
	
	Method Free()
		FreeShaderObject_( instance )
	End Method

End Type