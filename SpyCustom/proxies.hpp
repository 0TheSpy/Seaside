#pragma once


RecvVarProxyFn fnSequenceProxyFn = NULL;

void SetViewModelSequence(const CRecvProxyData* pDataConst, void* pStruct, void* pOut) {

	CRecvProxyData* pData = const_cast<CRecvProxyData*>(pDataConst);

		C_BaseViewModel* pViewModel = (C_BaseViewModel*)pStruct;

		if (pViewModel) {
			IClientEntity* pOwner = iff.g_pEntityList->GetClientEntityFromHandle(pViewModel->GetOwner());

			if (pOwner && pOwner->GetIndex() == iff.g_pEngineClient->GetLocalPlayer()) {

				

				const model_t* pModel = iff.g_pMdlInfo->GetModel(pViewModel->GetModelIndex());
				const char* szModel = iff.g_pMdlInfo->GetModelName(pModel);

				int m_nSequence = pData->m_Value.m_Int;

				auto model = fnv2::hashRuntime(szModel);

				static int defknifethash = fnv2::hash("models/weapons/v_knife_default_t.mdl");
				static int defknifecthash = fnv2::hash("models/weapons/v_knife_default_ct.mdl");
				if (model == defknifethash || model == defknifecthash) model = fnv2::hash(g_Options.weapons.value->arr[0].model);
				  
				for (int i = 2; i < g_Options.models.value->itemcount; i++) {

					if (model == g_Options.models.value->arr[i].vmodel_hash)
					{
						if (g_Options.models.value->arr[i].seq_active)
							m_nSequence = g_Options.models.value->arr[i].seqs[m_nSequence].seq_repl;
						break;
					}
					if (model == fnv2::hash(g_Options.models.value->arr[i].vmodel_orig))
					{
						if (g_Options.models.value->arr[i].seq_active)   
							m_nSequence = g_Options.models.value->arr[i].seqs[m_nSequence].seq_repl;
						
						break;
					}
				}

				switch (model) {
				case fnv2::hash("models/weapons/v_knife_butterfly.mdl"):
				{
					switch (m_nSequence)
					{
					case SEQUENCE_DEFAULT_DRAW:
						m_nSequence = random(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2); break;
					case SEQUENCE_DEFAULT_LOOKAT01:
						m_nSequence = random(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03); break;
					default:
						m_nSequence++;
					}
					break;
				}


				case fnv2::hash("models/weapons/v_knife_falchion_advanced.mdl"):
				{
					switch (m_nSequence)
					{
					case SEQUENCE_DEFAULT_IDLE2:
						m_nSequence = SEQUENCE_FALCHION_IDLE1; break;
					case SEQUENCE_DEFAULT_HEAVY_MISS1:
						m_nSequence = random(SEQUENCE_FALCHION_HEAVY_MISS1, SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP); break;
					case SEQUENCE_DEFAULT_LOOKAT01:
						m_nSequence = random(SEQUENCE_FALCHION_LOOKAT01, SEQUENCE_FALCHION_LOOKAT02); break;
					case SEQUENCE_DEFAULT_DRAW:
					case SEQUENCE_DEFAULT_IDLE1:
						m_nSequence = m_nSequence; break;
					default:
						m_nSequence--;
					}
					break;
				}


				case fnv2::hash("models/weapons/v_knife_push.mdl"):
				{
					switch (m_nSequence)
					{
					case SEQUENCE_DEFAULT_IDLE2:
						m_nSequence = SEQUENCE_DAGGERS_IDLE1; break;
					case SEQUENCE_DEFAULT_LIGHT_MISS1:
					case SEQUENCE_DEFAULT_LIGHT_MISS2:
						m_nSequence = random(SEQUENCE_DAGGERS_LIGHT_MISS1, SEQUENCE_DAGGERS_LIGHT_MISS5); break;
					case SEQUENCE_DEFAULT_HEAVY_MISS1:
						m_nSequence = random(SEQUENCE_DAGGERS_HEAVY_MISS2, SEQUENCE_DAGGERS_HEAVY_MISS1); break;
					case SEQUENCE_DEFAULT_HEAVY_HIT1:
					case SEQUENCE_DEFAULT_HEAVY_BACKSTAB:
					case SEQUENCE_DEFAULT_LOOKAT01:
						m_nSequence = m_nSequence + 3; break;
					case SEQUENCE_DEFAULT_DRAW:
					case SEQUENCE_DEFAULT_IDLE1:
						m_nSequence = m_nSequence; break;
					default:
						m_nSequence = m_nSequence + 2;
					}
					break;
				}

				case fnv2::hash("models/weapons/v_knife_survival_bowie.mdl"):  
				{
					switch (m_nSequence)
					{
					case SEQUENCE_DEFAULT_DRAW:
					case SEQUENCE_DEFAULT_IDLE1:
						m_nSequence = m_nSequence; break;
					case SEQUENCE_DEFAULT_IDLE2:
						m_nSequence = SEQUENCE_BOWIE_IDLE1; break;
					default:
						m_nSequence = m_nSequence - 1;
					}
					break;
				}

				case fnv2::hash("models/weapons/v_knife_ursus.mdl"):
				case fnv2::hash("models/weapons/v_knife_skeleton.mdl"):
				case fnv2::hash("models/weapons/v_knife_outdoor.mdl"):
				case fnv2::hash("models/weapons/v_knife_cord.mdl"):
				case fnv2::hash("models/weapons/v_knife_canis.mdl"):
				{
					switch (m_nSequence)
					{
					case SEQUENCE_DEFAULT_DRAW:
						m_nSequence = random(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2); break;
					case SEQUENCE_DEFAULT_LOOKAT01:
						m_nSequence = random(SEQUENCE_BUTTERFLY_LOOKAT01, 14); break;
					default:
						m_nSequence = m_nSequence + 1;
					}
					break;
				}

				case fnv2::hash("models/weapons/v_knife_stiletto.mdl"):
				{
					switch (m_nSequence)
					{
					case SEQUENCE_DEFAULT_LOOKAT01:
						m_nSequence = random(12, 13); break;
					}
					break;
				}

				case fnv2::hash("models/weapons/v_knife_widowmaker.mdl"):
				{
					switch (m_nSequence)
					{
					case SEQUENCE_DEFAULT_LOOKAT01:
						m_nSequence = random(14, 15); break;
					}
					break;
				}

				case fnv2::hash("models/weapons/v_fists.mdl"):
				case fnv2::hash("models/weapons/v_axe.mdl"):
				case fnv2::hash("models/weapons/v_hammer.mdl"):
				case fnv2::hash("models/weapons/v_spanner.mdl"):
				{
					static int lastpunch = 3;
					switch (m_nSequence)
					{
					case SEQUENCE_DEFAULT_DRAW:
						m_nSequence = 1; break; 
					case 1:
					case 2:
					case 12:
						m_nSequence = 0; break;
					default:
						if (lastpunch == 3) lastpunch = 2; else lastpunch = 3;
						m_nSequence = lastpunch;
					}

					break;
				}

				}

#ifdef DEBUG
				cout << "active " << (char*)pViewModel->GetSequenceActivity(m_nSequence);
				printf(" ~new seq %d (%s)\n ", m_nSequence, szModel);
#endif


				pData->m_Value.m_Int = m_nSequence;
			}
		}

	fnSequenceProxyFn(pData, pStruct, pOut);
}
