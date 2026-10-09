// BismIllahIRRahmaanIRRaheem
/* d3d11 scene impl class implementation */

#include <graphics/graphicsdef.h>
#ifdef FLX_TRY_D3D11
#include <d3d11scene.h>
#include <rtd3d11debug.h>

static uint32 g_VertexShaderBytecodeAlbedo[] = { 1128421444u, 2402879077u, 4141149369u, 3236055827u, 3179830628u, 1u, 1820u, 6u, 56u, 584u,
1316u, 1440u, 1648u, 1732u, 963538753u, 520u, 520u, 4294836736u, 468u, 52u,
2359297u, 3145728u, 3145728u, 2359296u, 3145729u, 0u, 65540u, 0u, 0u, 4294836736u,
83886161u, 2685337605u, 1056964608u, 1065353216u, 0u, 0u, 33554463u, 2147483653u, 2416902144u, 33554463u,
2147549189u, 2416902145u, 33554433u, 2147549184u, 2684354565u, 50331653u, 2147876864u, 2147483648u, 2700607489u, 50331650u,
2147549185u, 2158624768u, 2153054208u, 50331653u, 2147876864u, 2147483648u, 2700607490u, 50331650u, 2147614721u, 2158624768u,
2153054208u, 50331653u, 2147876864u, 2147483648u, 2700607491u, 50331650u, 2147745793u, 2158624768u, 2153054208u, 50331653u,
2147680256u, 2147483648u, 2699952132u, 50331650u, 2148007937u, 2153054208u, 2147483648u, 67108868u, 2148466688u, 2418278400u,
2694119429u, 2691301381u, 50331657u, 3221487616u, 2162425857u, 2162425856u, 33554433u, 2147549185u, 2684354561u, 33554433u,
2147614721u, 2684354562u, 33554433u, 2147745793u, 2684354563u, 33554433u, 2148007937u, 2684354564u, 50331657u, 2147549185u,
2162425857u, 2162425856u, 33554433u, 2147549186u, 2689925121u, 33554433u, 2147614722u, 2689925122u, 33554433u, 2147745794u,
2689925123u, 33554433u, 2148007938u, 2689925124u, 50331657u, 2147614721u, 2162425858u, 2162425856u, 33554433u, 2147549186u,
2701066241u, 33554433u, 2147614722u, 2701066242u, 33554433u, 2147745794u, 2701066243u, 33554433u, 2148007938u, 2701066244u,
50331657u, 2147549184u, 2162425858u, 2162425856u, 67108868u, 3221422080u, 2147483648u, 2699296768u, 2162425857u, 33554433u,
3221749760u, 2147483648u, 33554433u, 3758292992u, 2430861313u, 65535u, 1380206675u, 724u, 65600u, 181u,
67108953u, 2133574u, 0u, 4u, 50331743u, 1052786u, 0u, 50331743u, 1052722u, 1u,
67108967u, 1057010u, 0u, 1u, 50331749u, 1056818u, 1u, 33554536u, 2u, 100663350u,
1048594u, 0u, 2129930u, 0u, 0u, 100663350u, 1048610u, 0u, 2129930u, 0u,
1u, 100663350u, 1048642u, 0u, 2129930u, 0u, 2u, 100663350u, 1048706u, 0u,
2129930u, 0u, 3u, 83886134u, 1048690u, 1u, 1053254u, 0u, 83886134u, 1048706u,
1u, 16385u, 1065353216u, 117440529u, 1056786u, 0u, 1052230u, 0u, 1052230u, 1u,
100663350u, 1048594u, 0u, 2129946u, 0u, 0u, 100663350u, 1048610u, 0u, 2129946u,
0u, 1u, 100663350u, 1048642u, 0u, 2129946u, 0u, 2u, 100663350u, 1048706u,
0u, 2129946u, 0u, 3u, 117440529u, 1056802u, 0u, 1052230u, 0u, 1052230u,
1u, 184549391u, 1048594u, 0u, 16386u, 1056964608u, 1056964608u, 0u, 0u, 2132710u,
0u, 0u, 184549391u, 1048610u, 0u, 16386u, 1056964608u, 1056964608u, 0u, 0u,
2132710u, 0u, 1u, 184549391u, 1048642u, 0u, 16386u, 1056964608u, 1056964608u, 0u,
0u, 2132710u, 0u, 2u, 184549391u, 1048706u, 0u, 16386u, 1056964608u, 1056964608u,
0u, 0u, 2132710u, 0u, 3u, 117440529u, 1056834u, 0u, 1052230u, 0u,
1052230u, 1u, 100663350u, 1048594u, 0u, 2129978u, 0u, 0u, 100663350u, 1048610u,
0u, 2129978u, 0u, 1u, 100663350u, 1048642u, 0u, 2129978u, 0u, 2u,
100663350u, 1048706u, 0u, 2129978u, 0u, 3u, 117440529u, 1056898u, 0u, 1052230u,
0u, 1052230u, 1u, 83886134u, 1056818u, 1u, 1052742u, 1u, 16777278u, 1413567571u,
116u, 24u, 2u, 0u, 4u, 8u, 0u, 0u, 1u, 0u,
0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u,
15u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u,
1178944594u, 200u, 1u, 72u, 1u, 28u, 4294837248u, 2304u, 160u, 60u,
0u, 0u, 0u, 0u, 0u, 1u, 1u, 1702129225u, 1801539698u, 2880154368u,
60u, 1u, 96u, 64u, 0u, 0u, 120u, 0u, 64u, 2u,
144u, 0u, 1851880020u, 1919903347u, 1769234797u, 1632464495u, 2020176500u, 2880154368u, 196611u, 262148u,
0u, 0u, 1919117645u, 1718580079u, 1378361460u, 1279795241u, 1394625619u, 1701077352u, 1866670194u, 1818849389u,
824210021u, 3223088u, 1313297225u, 76u, 2u, 8u, 56u, 0u, 0u, 3u,
0u, 1799u, 65u, 0u, 0u, 3u, 1u, 771u, 1230196560u, 1313818964u,
1480938496u, 1380929347u, 2880110660u, 1313297231u, 80u, 2u, 8u, 56u, 0u, 1u,
3u, 0u, 15u, 68u, 0u, 0u, 3u, 1u, 3075u, 1348425299u,
1414091599u, 5132105u, 1129858388u, 1146244943u, 2880154368u, };

static uint32 g_PixelShaderBytecodeAlbedo[] = { 1128421444u, 1375268566u, 1200379889u, 3503995554u, 238599292u, 1u, 696u, 6u, 56u, 164u,
272u, 396u, 556u, 644u, 963538753u, 100u, 100u, 4294902272u, 60u, 40u,
2621440u, 2621440u, 2621440u, 2359297u, 2621440u, 0u, 4294902272u, 33554463u, 2147483648u, 2952986624u,
33554463u, 2415919104u, 2685339648u, 50331714u, 2148466688u, 2967732224u, 2699298816u, 33554433u, 2148468736u, 2162425856u,
65535u, 1380206675u, 100u, 64u, 25u, 50331738u, 1073152u, 0u, 67115096u, 1077248u,
0u, 21845u, 50335842u, 1052722u, 1u, 50331749u, 1057010u, 0u, 150995013u, 1057010u,
0u, 1052742u, 1u, 1080902u, 0u, 1073152u, 0u, 16777278u, 1413567571u, 116u,
2u, 0u, 0u, 2u, 0u, 0u, 0u, 1u, 0u, 0u,
0u, 0u, 0u, 0u, 1u, 0u, 0u, 0u, 0u, 0u,
0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 1178944594u,
152u, 0u, 0u, 2u, 28u, 4294902784u, 2304u, 112u, 92u, 3u,
0u, 0u, 0u, 0u, 1u, 1u, 102u, 2u, 5u, 4u,
4294967295u, 0u, 1u, 13u, 1632853863u, 1701605485u, 1600585842u, 1954047316u, 6648437u, 1919117645u,
1718580079u, 1378361460u, 1279795241u, 1394625619u, 1701077352u, 1866670194u, 1818849389u, 824210021u, 3223088u, 1313297225u,
80u, 2u, 8u, 56u, 0u, 1u, 3u, 0u, 15u, 68u,
0u, 0u, 3u, 1u, 771u, 1348425299u, 1414091599u, 5132105u, 1129858388u, 1146244943u,
2880154368u, 1313297231u, 44u, 1u, 8u, 32u, 0u, 0u, 3u, 0u,
15u, 1415534163u, 1162302017u, 2880110676u, };

bit falx::D3D11GraphicsScene::Create(IGraphicsDevice* aiGraphicsDevice)
{
	FLX_SMART_CHECK(aiGraphicsDevice != NULL, "Invalid graphics device fed into the scene system implementation");
	i_GraphicsDevice = (D3D11GraphicsDevice*)aiGraphicsDevice;
	ID3D11Device* i_Device = i_GraphicsDevice->GetDevice();
	FLX_SMART_CHECK_HRESULT(i_Device->CreateVertexShader(g_VertexShaderBytecodeAlbedo, sizeof(g_VertexShaderBytecodeAlbedo), NULL, &i_AlbedoVertexShader), "Albedo vertex shader creation failed");
	FLX_SMART_CHECK_HRESULT(i_Device->CreatePixelShader(g_PixelShaderBytecodeAlbedo, sizeof(g_PixelShaderBytecodeAlbedo), NULL, &i_AlbedoPixelShader), "Albedo pixel shader creation failed");
	{
		D3D11_INPUT_ELEMENT_DESC m_InputLayoutDesc[2];
		FLX_ZMEM(&m_InputLayoutDesc, sizeof(D3D11_INPUT_ELEMENT_DESC) * 2);
		m_InputLayoutDesc[0].SemanticName = "POSITION";
		m_InputLayoutDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		m_InputLayoutDesc[1].SemanticName = "TEXCOORD";
		m_InputLayoutDesc[1].Format = DXGI_FORMAT_R32G32_FLOAT;
		m_InputLayoutDesc[1].AlignedByteOffset = 12;
		FLX_SMART_CHECK_HRESULT(i_Device->CreateInputLayout(m_InputLayoutDesc, 2, g_VertexShaderBytecodeAlbedo, sizeof(g_VertexShaderBytecodeAlbedo), &i_InputLayout), "Albedo input layout creation failed");
	}
	{
		D3D11_BUFFER_DESC m_ConstantBufferDesc;
		FLX_ZMEM(&m_ConstantBufferDesc, sizeof(D3D11_BUFFER_DESC));
		m_ConstantBufferDesc.ByteWidth = (UINT)sizeof(matrix4x4);
		m_ConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		FLX_SMART_CHECK_HRESULT(i_Device->CreateBuffer(&m_ConstantBufferDesc, NULL, &i_AlbedoConstantBuffer), "Albedo constant buffer creation failed");
	}
	{
		static const uint32 c_WhitePixel = 0xFFFFFFFFu;
		D3D11_TEXTURE2D_DESC m_WhiteTextureDesc;
		FLX_ZMEM(&m_WhiteTextureDesc, sizeof(D3D11_TEXTURE2D_DESC));
		m_WhiteTextureDesc.Width = 1;
		m_WhiteTextureDesc.Height = 1;
		m_WhiteTextureDesc.MipLevels = 1;
		m_WhiteTextureDesc.ArraySize = 1;
		m_WhiteTextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		m_WhiteTextureDesc.SampleDesc.Count = 1;
		m_WhiteTextureDesc.SampleDesc.Quality = 0;
		m_WhiteTextureDesc.Usage = D3D11_USAGE_IMMUTABLE;
		m_WhiteTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		D3D11_SUBRESOURCE_DATA m_WhiteTextureData;
		FLX_ZMEM(&m_WhiteTextureData, sizeof(D3D11_SUBRESOURCE_DATA));
		m_WhiteTextureData.pSysMem = &c_WhitePixel;
		m_WhiteTextureData.SysMemPitch = sizeof(uint32);
		FLX_SMART_CHECK_HRESULT(i_Device->CreateTexture2D(&m_WhiteTextureDesc, &m_WhiteTextureData, &i_WhiteTexture), "White texture creation failed");
		D3D11_SHADER_RESOURCE_VIEW_DESC m_WhiteTextureSRVDesc;
		FLX_ZMEM(&m_WhiteTextureSRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		m_WhiteTextureSRVDesc.Format = m_WhiteTextureDesc.Format;
		m_WhiteTextureSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		m_WhiteTextureSRVDesc.Texture2D.MipLevels = 1;
		FLX_SMART_CHECK_HRESULT(i_Device->CreateShaderResourceView(i_WhiteTexture, &m_WhiteTextureSRVDesc, &i_WhiteTextureSRV), "White texture SRV creation failed");
	}
	return true;
}

falx::ObjectID falx::D3D11GraphicsScene::CreateObject(ulargeint aVertexBufferBytes, ulargeint aIndexBufferBytes, uint32 aStride)
{
	FLX_SMART_CHECK(aVertexBufferBytes > 0, "Vertex buffer size must be greater than zero");
	FLX_SMART_CHECK(aIndexBufferBytes > 0, "Index buffer size must be greater than zero");
	ID3D11Buffer* i_VertexBuffer;
	ID3D11Buffer* i_IndexBuffer;
	{
		D3D11_BUFFER_DESC m_VertexBufferDesc;
		FLX_ZMEM(&m_VertexBufferDesc, sizeof(D3D11_BUFFER_DESC));
		m_VertexBufferDesc.ByteWidth = (UINT)aVertexBufferBytes;
		m_VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		FLX_SMART_CHECK_HRESULT(i_GraphicsDevice->GetDevice()->CreateBuffer(&m_VertexBufferDesc, NULL, &i_VertexBuffer), "Vertex buffer initialization failed");
	}
	{
		D3D11_BUFFER_DESC m_IndexBufferDesc;
		FLX_ZMEM(&m_IndexBufferDesc, sizeof(D3D11_BUFFER_DESC));
		m_IndexBufferDesc.ByteWidth = (UINT)aIndexBufferBytes;
		m_IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		FLX_SMART_CHECK_HRESULT(i_GraphicsDevice->GetDevice()->CreateBuffer(&m_IndexBufferDesc, NULL, &i_IndexBuffer), "Index buffer initialization failed");
	}
	D3D11Object* m_Object = new D3D11Object;
	m_Object->i_IndexBuffer = i_IndexBuffer;
	m_Object->i_VertexBuffer = i_VertexBuffer;
	m_Object->m_IndexBufferBytes = aIndexBufferBytes;
	m_Object->m_VertexBufferBytes = aVertexBufferBytes;
	m_Objects.push_back(m_Object);
	return { m_Objects.size() };
}


void falx::D3D11GraphicsScene::DismissObject(ObjectID aObject)
{
	FLX_SMART_CHECK(aObject.ID != NULL, "Invalid object referenced in graphics scene - CSNULL");
	FLX_SMART_CHECK(aObject.ID <= m_Objects.size(), "Invalid object referenced in graphics scene - CSOVFL");
	D3D11Object* p_Object = m_Objects[aObject.ID - 1];
	FLX_SMART_CHECK(p_Object != NULL, "Object referenced in graphics scene was already dismissed - CSDISM");
	p_Object->i_IndexBuffer->Release();
	p_Object->i_VertexBuffer->Release();
	delete p_Object;
	m_Objects[aObject.ID - 1] = NULL;
}

void falx::D3D11GraphicsScene::AlbedoRender(ClumpID aClump)
{
	FLX_SMART_CHECK(aClump.FatherID != NULL, "Invalid object referenced in graphics scene - CSNULL");
	FLX_SMART_CHECK(aClump.FatherID <= m_Objects.size(), "Invalid object referenced in graphics scene - CSOVFL");
	D3D11Object* p_Object = m_Objects[aClump.FatherID - 1];
	if (p_Object == NULL) {
		return;
	}
	FLX_SMART_CHECK(aClump.ID != NULL && aClump.ID <= p_Object->m_Clumps.size(), "Invalid clump referenced in graphics scene");
	const ClearClumpD3D11& r_ClClump = p_Object->m_Clumps[aClump.ID - 1];
	FLX_SMART_CHECK(aClump.ID == 0, "Invalid clump referenced as an argument to AlbedoRender");
	ID3D11DeviceContext* i_ImmediateContext = i_GraphicsDevice->GetContext();
	i_ImmediateContext->PSSetShader(i_AlbedoPixelShader, NULL, 0);
	i_ImmediateContext->PSSetShaderResources(0, 1, &i_WhiteTextureSRV);
	i_ImmediateContext->VSSetShader(i_AlbedoVertexShader, NULL, 0);
	i_ImmediateContext->VSSetConstantBuffers(0, 1, &i_AlbedoConstantBuffer);
	i_ImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	i_ImmediateContext->IASetInputLayout(i_InputLayout);
	{
		UINT m_Stride = 20;
		UINT m_Offset = 0;
		i_ImmediateContext->IASetIndexBuffer(p_Object->i_IndexBuffer, DXGI_FORMAT_R16_UINT, 0);
		i_ImmediateContext->IASetVertexBuffers(0, 1, &p_Object->i_VertexBuffer, &m_Stride, &m_Offset);
	}
	i_ImmediateContext->UpdateSubresource(i_AlbedoConstantBuffer, 0, NULL, &r_ClClump.Metadata.Transform, 0, 0);
	i_ImmediateContext->DrawIndexed(r_ClClump.Metadata.NumIndices, r_ClClump.Metadata.StartIndex, 0);
}

void falx::D3D11GraphicsScene::AlbedoRender(ObjectID aObject)
{
	FLX_SMART_CHECK(aObject.ID != NULL, "Invalid object referenced in graphics scene - CSNULL");
	FLX_SMART_CHECK(aObject.ID <= m_Objects.size(), "Invalid object referenced in graphics scene - CSOVFL");
	D3D11Object* p_Object = m_Objects[aObject.ID - 1];
	FLX_SMART_CHECK(p_Object != NULL, "Object referenced in graphics scene was already dismissed - CSDISM");
	ID3D11DeviceContext* i_ImmediateContext = i_GraphicsDevice->GetContext();
	i_ImmediateContext->PSSetShader(i_AlbedoPixelShader, NULL, 0);
	i_ImmediateContext->PSSetShaderResources(0, 1, &i_WhiteTextureSRV);
	i_ImmediateContext->VSSetShader(i_AlbedoVertexShader, NULL, 0);
	i_ImmediateContext->VSSetConstantBuffers(0, 1, &i_AlbedoConstantBuffer);
	i_ImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	i_ImmediateContext->IASetInputLayout(i_InputLayout);
	{
		UINT m_Stride = 20;
		UINT m_Offset = 0;
		i_ImmediateContext->IASetIndexBuffer(p_Object->i_IndexBuffer, DXGI_FORMAT_R16_UINT, 0);
		i_ImmediateContext->IASetVertexBuffers(0, 1, &p_Object->i_VertexBuffer, &m_Stride, &m_Offset);
	}
	for (const ClearClumpD3D11& r_ClClump : p_Object->m_Clumps) {
		i_ImmediateContext->UpdateSubresource(i_AlbedoConstantBuffer, 0, NULL, &r_ClClump.Metadata.Transform, 0, 0);
		if (r_ClClump.Metadata.Parent.ID == NULL)
			continue;
		i_ImmediateContext->DrawIndexed(r_ClClump.Metadata.NumIndices, r_ClClump.Metadata.StartIndex, 0);
	}
}

void falx::D3D11GraphicsScene::AlbedoRender()
{
	for (size_t i = 0; i < m_Objects.size(); i++) {
		if (m_Objects[i] == NULL)
			continue;
		D3D11Object* p_Object = m_Objects[i];
		ID3D11DeviceContext* i_ImmediateContext = i_GraphicsDevice->GetContext();
		i_ImmediateContext->PSSetShader(i_AlbedoPixelShader, NULL, 0);
		i_ImmediateContext->PSSetShaderResources(0, 1, &i_WhiteTextureSRV);
		i_ImmediateContext->VSSetShader(i_AlbedoVertexShader, NULL, 0);
		i_ImmediateContext->VSSetConstantBuffers(0, 1, &i_AlbedoConstantBuffer);
		i_ImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		i_ImmediateContext->IASetInputLayout(i_InputLayout);
		{
			UINT m_Stride = 20;
			UINT m_Offset = 0;
			i_ImmediateContext->IASetIndexBuffer(p_Object->i_IndexBuffer, DXGI_FORMAT_R16_UINT, 0);
			i_ImmediateContext->IASetVertexBuffers(0, 1, &p_Object->i_VertexBuffer, &m_Stride, &m_Offset);
		}
		for (const ClearClumpD3D11& r_ClClump : p_Object->m_Clumps) {
			i_ImmediateContext->UpdateSubresource(i_AlbedoConstantBuffer, 0, NULL, &r_ClClump.Metadata.Transform, 0, 0);
			if (r_ClClump.Metadata.Parent.ID == NULL)
				continue;
			i_ImmediateContext->DrawIndexed(r_ClClump.Metadata.NumIndices, r_ClClump.Metadata.StartIndex, 0);
		}
	}
}

static falx::ClearClumpD3D11* GetClump(std::vector<falx::D3D11Object*>& aObjects, falx::ClumpID aClump)
{
	FLX_SMART_CHECK(aClump.FatherID != NULL && aClump.FatherID <= aObjects.size(), "Invalid clump parent object referenced in graphics scene");
	falx::D3D11Object* p_Object = aObjects[aClump.FatherID - 1];
	FLX_SMART_CHECK(p_Object != NULL, "Clump parent object was dismissed");
	FLX_SMART_CHECK(aClump.ID != NULL && aClump.ID <= p_Object->m_Clumps.size(), "Invalid clump referenced in graphics scene");
	falx::ClearClumpD3D11* p_Clump = &p_Object->m_Clumps[aClump.ID - 1];
	FLX_SMART_CHECK(p_Clump->Metadata.Parent.ID != NULL, "Clump was dismissed");
	return p_Clump;
}

static void UploadBytes(ID3D11DeviceContext* aiContext, ID3D11Buffer* aiBuffer, ulargeint aOffset, ulargeint aBytes, const void* apData)
{
	D3D11_BOX m_Box;
	m_Box.left = (UINT)aOffset;
	m_Box.right = (UINT)(aOffset + aBytes);
	m_Box.top = 0;
	m_Box.bottom = 1;
	m_Box.front = 0;
	m_Box.back = 1;
	aiContext->UpdateSubresource(aiBuffer, 0, &m_Box, apData, 0, 0);
}

falx::ClumpID falx::D3D11GraphicsScene::CreateClump(CLUMP_METADATA aMetadata, float32* apVertexStartData, uint16* apIndexStartData)
{
	FLX_SMART_CHECK(aMetadata.Parent.ID != NULL && aMetadata.Parent.ID <= m_Objects.size(), "Invalid parent object referenced for clump creation");
	D3D11Object* p_Object = m_Objects[aMetadata.Parent.ID - 1];
	FLX_SMART_CHECK(p_Object != NULL, "Parent object was dismissed");
	FLX_SMART_CHECK(aMetadata.StartVertexBufferBytes + aMetadata.NumVertexBufferBytes <= p_Object->m_VertexBufferBytes, "Clump vertex data exceeds parent object vertex buffer");
	FLX_SMART_CHECK(aMetadata.StartIndexBufferBytes + aMetadata.NumIndexBufferBytes <= p_Object->m_IndexBufferBytes, "Clump index data exceeds parent object index buffer");
	ID3D11DeviceContext* i_Context = i_GraphicsDevice->GetContext();
	if (apVertexStartData != NULL && aMetadata.NumVertexBufferBytes > 0)
		UploadBytes(i_Context, p_Object->i_VertexBuffer, aMetadata.StartVertexBufferBytes, aMetadata.NumVertexBufferBytes, apVertexStartData);
	if (apIndexStartData != NULL && aMetadata.NumIndexBufferBytes > 0)
		UploadBytes(i_Context, p_Object->i_IndexBuffer, aMetadata.StartIndexBufferBytes, aMetadata.NumIndexBufferBytes, apIndexStartData);
	ClearClumpD3D11 m_Clump;
	m_Clump.Metadata = aMetadata;
	p_Object->m_Clumps.push_back(m_Clump);
	return { aMetadata.Parent.ID, p_Object->m_Clumps.size() };
}

void falx::D3D11GraphicsScene::UpdateClump(ClumpID aClump, matrix4x4 aTransform)
{
	GetClump(m_Objects, aClump)->Metadata.Transform = aTransform;
}

void falx::D3D11GraphicsScene::UpdateClumpNumVertices(ClumpID aClump, uint16 aNumVertices)
{
	GetClump(m_Objects, aClump)->Metadata.NumVertices = aNumVertices;
}

void falx::D3D11GraphicsScene::UpdateClumpNumIndices(ClumpID aClump, uint32 aNumIndices)
{
	GetClump(m_Objects, aClump)->Metadata.NumIndices = aNumIndices;
}

void falx::D3D11GraphicsScene::UpdateClump(ClumpID aClump, float32* apVertexData, ulargeint aCurrentAttemptedAllocationSize)
{
	FLX_SMART_CHECK(apVertexData != NULL, "Invalid vertex data fed into clump update");
	ClearClumpD3D11* p_Clump = GetClump(m_Objects, aClump);
	FLX_SMART_CHECK(aCurrentAttemptedAllocationSize <= p_Clump->Metadata.NumVertexBufferBytes, "Clump vertex update exceeds clump allocation");
	UploadBytes(i_GraphicsDevice->GetContext(), m_Objects[aClump.FatherID - 1]->i_VertexBuffer, p_Clump->Metadata.StartVertexBufferBytes, aCurrentAttemptedAllocationSize, apVertexData);
}

void falx::D3D11GraphicsScene::UpdateClump(ClumpID aClump, uint16* apIndexData, ulargeint aCurrentAttemptedAllocationSize)
{
	FLX_SMART_CHECK(apIndexData != NULL, "Invalid index data fed into clump update");
	ClearClumpD3D11* p_Clump = GetClump(m_Objects, aClump);
	FLX_SMART_CHECK(aCurrentAttemptedAllocationSize <= p_Clump->Metadata.NumIndexBufferBytes, "Clump index update exceeds clump allocation");
	UploadBytes(i_GraphicsDevice->GetContext(), m_Objects[aClump.FatherID - 1]->i_IndexBuffer, p_Clump->Metadata.StartIndexBufferBytes, aCurrentAttemptedAllocationSize, apIndexData);
}

void falx::D3D11GraphicsScene::DismissClump(ClumpID& aClump)
{
	aClump.ID = 0;
}

void falx::D3D11GraphicsScene::Dismiss()
{
	for (size_t i = 0; i < m_Objects.size(); i++) {
		D3D11Object* p_Object = m_Objects[i];
		if (p_Object == NULL) continue;
		p_Object->i_IndexBuffer->Release();
		p_Object->i_VertexBuffer->Release();
		delete p_Object;
	}
	m_Objects.clear();
	
	i_WhiteTextureSRV->Release();
	i_WhiteTexture->Release();
	i_AlbedoConstantBuffer->Release();
	i_AlbedoPixelShader->Release();
	i_AlbedoVertexShader->Release();
	i_InputLayout->Release();
}

#endif // FLX_TRY_D3D11


