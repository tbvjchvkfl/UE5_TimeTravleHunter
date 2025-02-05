Contents
-

### [1. Inventory System](#inventory-system)
### [2. Equipment System](#equipment-system)
### [3. Item Pool](#item-pool)

</br>

Inventory System
-
Unreal Engine 자료구조 TMap을 활용한 레지던트 이블 스타일의 공간 인벤토리와 WrapBox를 활용한 슬롯 인벤토리입니다.
Mouse Hover, Click에 의한 상호작용 기능을 구현했습니다.


예시 영상
-
https://github.com/user-attachments/assets/ad5c97a2-220e-4e43-8d67-c58303ac12f3

클래스 구조도
-
![Image](https://github.com/user-attachments/assets/6c8cf380-cdb6-4a19-835b-2c0b45f3f876)

핵심 기능
-

- ### Inventory Component
  > - 언리얼 엔진 TMap을 사용해서 인벤토리 내 위치와 그 위치에 따른 값을 가지도록 구현했습니다. 
  > - 인벤토리가 가지고있을 슬롯의 개수 Size와 한 행에 들어갈 슬롯의 개수 Width를 지정하였고, 현재 인벤토리의 공간이 비어있는지 채워져있는지를 판단하기 위해 TMap<FVector2D, bool> InventoryState를 만들어주었습니다.
  > - 게임 내에서 재화를 습득했을 경우 인벤토리의 슬롯이 아닌 단순 텍스트로 별도 표시해줄 생각이었으므로 CoinInventory라는 변수를 만들어 주었고, APickUpItem에 아이템 타입을 Enum class로 지정하여 구분해주었습니다.
  <pre>
   <code>
      UPROPERTY(EditAnywhere, Category = "Inventory | Property", meta = (AllowPrivateAccess = "true"))
      int32 InventorySize;
      
      UPROPERTY(EditAnywhere, Category = "Inventory | Property", meta = (AllowPrivateAccess = "true"))
      int32 InventoryWidth;
      
      UPROPERTY(VisibleAnywhere, Category = "Inventory | Property", meta = (AllowPrivateAccess = "true"))
      TMap<FVector2D, APickUpItem *> ItemInventory;
      
      UPROPERTY(VisibleAnywhere, Category = "Inventory | Property", meta = (AllowPrivateAccess = "true"))
      TMap<FVector2D, bool> InventoryState;
      
      UPROPERTY()
      int32 CoinInventory;
   </code>
  </pre>

    > - 캐릭터가 APickUpItem과 인터렉션 하면 CheckItem 함수를 통해 Item의 타입을 확인하여 타입에 따라 별도 함수를 호출해주었습니다.
    > - 멀티 캐스트 델리게이트를 바인딩하여 아이템을 획득할 때마다 위젯 인벤토리의 내용이 업데이트 될 수 있도록 구현했습니다.
    <pre>
   <code>
      void UInventoryComponent::CheckItem(class APickUpItem *Item)
      {
      	switch (Item->GetItemType())
      	{
      		case EItemType::Coin:
      		{
      			CoinInventory += Item->GetCurrentQuantity();
      		}
      		break;
      		case EItemType::Weapon:
      		{
      			AddWeapon(Item);
      		}
      		break;
      		case EItemType::Consumable:
      		{
      			AddItem(Item);
      		}
      		break;
      		default:
      		{
      			return;
      		}
      		break;
      	}
      	OnInventoryUpdate.Broadcast();
      }
   </code>
  </pre>
  
    > - CheckItem 이후, AddItem 함수가 호출 되면 우선적으로 ItemInventory를 순회하며 Inventory안에 Item이 있거나, 해당 아이템의 값이 nullptr이 아닐 경우 아이템 중복 체크를 위해 만들어둔 ItemNumber를 서로 비교하여 만약, 같은 아이템이라면 아이템의 수량만 증가되도록 구현하였습니다.
    > - Inventory가 비어있거나, 획득한 아이템이 기존의 아이템과 같은 아이템이 아니라면 bIsRoomAvailable함수를 호출하여 현재 Inventory에 획득한 아이템이 들어갈 수 있는 공간이 있는지 확인하였고, 해당 함수가 true를 리턴하면 아이템이 추가되도록 구현하였습니다.
        
  <pre>
   <code>
      void UInventoryComponent::AddItem(APickUpItem *Items)
      {
      	FVector2D ItemPos = FVector2D(0, 0);
      	TArray<FVector2D> AllKeys;
      	ItemInventory.GetKeys(AllKeys);
      	for (int32 i = 0; i < AllKeys.Num(); i++)
      	{
      		if (Items && ItemInventory[AllKeys[i]])
      		{
      			if (ItemInventory[AllKeys[i]]->GetItemNumber() == Items->GetItemNumber())
      			{
      				if (ItemInventory[AllKeys[i]]->GetMaxQuantity() > Items->GetCurrentQuantity())
      				{
      					int32 ItemQuantityReference = ItemInventory[AllKeys[i]]->GetCurrentQuantity() + Items->GetCurrentQuantity();
      
      					int32 RemainQuantity = ItemInventory[AllKeys[i]]->GetMaxQuantity() - ItemInventory[AllKeys[i]]->GetCurrentQuantity();
      
      					ItemInventory[AllKeys[i]]->SetCurrentQuantity(ItemQuantityReference);
      
      					AddtoInventory(AllKeys[i], ItemInventory[AllKeys[i]], false);
      
      					Items->SetCurrentQuantity(Items->GetCurrentQuantity() - RemainQuantity);
      					if (Items->GetCurrentQuantity() <= 0)
      					{
      						return;
      					}
      				}
      			}
      		}
      	}
      	if (Items && bIsRoomAvailable(Items->GetShape(0.0f), ItemPos))
      	{
      		AddtoInventory(ItemPos, Items, true);
      		return;
      	}
      	if (Items && bIsRoomAvailable(Items->GetShape(90.0f), ItemPos))
      	{
      		AddtoInventory(ItemPos, Items, true);
      		return;
      	}
      }
                  
      bool UInventoryComponent::bIsRoomAvailable(TArray<FVector2D> Shape, FVector2D &ItemPosition) const
      {
      	bool bIsFaild = false;
      	TArray<FVector2D> AllKeys;
      	InventoryState.GetKeys(AllKeys);
      	for (int32 i = 0; i < AllKeys.Num(); i++)
      	{
      		if (InventoryState.Contains(AllKeys[i]))
      		{
      			if (InventoryState[AllKeys[i]] == false)
      			{
      				for (const auto &ItemShapeElem : Shape)
      				{
      					if (!InventoryState.Contains(FVector2D(AllKeys[i].X + ItemShapeElem.X, AllKeys[i].Y + ItemShapeElem.Y)))
      					{
      						bIsFaild = true;
      						break;
      					}
      					if (InventoryState[FVector2D(AllKeys[i].X + ItemShapeElem.X, AllKeys[i].Y + ItemShapeElem.Y)] == true)
      					{
      						bIsFaild = true;
      						break;
      					}
      				}
      				if (bIsFaild)
      				{
      					bIsFaild = false;
      				}
      				else
      				{
      					ItemPosition = AllKeys[i];
      					return true;
      				}
      			}
      		}
      	}
      	ItemPosition = FVector2D(0, 0);
      	return false;
      }
   </code>
  </pre>

- ### Grid Inventory
  > - InventoryComponent에 있는 ItemInventory와 InventoryState를 GridInventory에서도 만들어 값을 전달받았고, 해당 자료구조들을 순회하며 Slot을 생성해주었습니다.
  > - Item과 Slot간 상호작용 시 CanPlaceItem을 호출하여 현재 선택한 아이템이 변경하려는 위치에 들어갈 수 있는가를 확인해주었습니다.
  > - CanPlaceItem에서는 UItemInventory 타입의 포인터를 이용하여 선택한 아이템의 현재 위치를 저장했고, GridState를 순회하며 이동하려는 위치에 이미 아이템이 있는지, 선택한 아이템의 크기가 이동하려는 위치에 들어갈 수 있는지를 확인해주었고, 이동하려는 위치에 이미 아이템이 있다면 해당 아이템과 현재 선택한 아이템이 같은 아이템인지를 확인하게 해서 같은 아이템이라면 수량 선택한 아이템의 수량을 이동하려는 위치에 있었던 아이템에 더해주는 방식으로 구현했습니다.
  <pre>
   <code>
      bool UInventoryGrid::CanPlaceItem() const
      {
      	if (MovingItem && IsValid(MovingItem))
      	{
      		APickUpItem *ItemReference = MovingItem->PickUpItems;
      		float ItemLoc_X;
      		float ItemLoc_Y;
      		MovingItem->GetCurrentGridLocation(ItemLoc_X, ItemLoc_Y);
      		FVector2D ItemLocation = FVector2D(ItemLoc_X, ItemLoc_Y);
      
      		TArray<FVector2D> ItemShape = ItemReference->GetShape(ItemReference->GetItemRotation());
      		for (auto ShapeElem : ItemShape)
      		{
      			FVector2D RoundedVector = FVector2D(FMath::RoundToInt(ItemLocation.X + ShapeElem.X), FMath::RoundToInt(ItemLocation.Y + ShapeElem.Y));
      			if (!GridState.Contains(RoundedVector))
      			{
      				return false;
      			}
      			if (GridState[RoundedVector] == true)
      			{
      				TArray<FVector2D> AllKeys;
      				GridInventory.GetKeys(AllKeys);
      				for (auto GridElem : AllKeys)
      				{
      					if (GridInventory[GridElem]->PickUpItems->GetItemNumber() == ItemReference->GetItemNumber())
      					{
      						if (GridInventory[GridElem]->IsOverlapping(RoundedVector))
      						{
      							int32 RemainQuantity;
      							if (!GridInventory[GridElem]->IsFullyStack(RemainQuantity))
      							{
      								RemainQuantity -= ItemReference->GetCurrentQuantity();
      								int32 ClampQuantity = FMath::Clamp(ItemReference->GetMaxQuantity() - RemainQuantity, 0, ItemReference->GetMaxQuantity());
      
      								GridInventory[GridElem]->ModifyQuantity(ClampQuantity);
      
      								MovingItem->ModifyQuantity(RemainQuantity * -1);
      
      								if (RemainQuantity >= 0)
      								{
      									ItemWidget->ModifyQuantity(RemainQuantity * -1);
      
      									GridInventory[GridElem]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
      									return false;
      								}
      							}
      						}
      					}
      					else
      					{
      						return false;
      					}
      				}
      				return false;
      			}
      		}
      		return true;
      	}
      	else
      	{
      		return false;
      	}
      }
   </code>
  </pre>

- ### ItemInventory
  > - APickUpItem클래스에 있는 ItemRotation값을 얻어온 후, SetRenderTranslation()함수를 활용하여 UItemInventory의 위젯 컴포넌트의 각 항목들의 각도를 변경해주었습니다.
  <pre>
   <code>
      void UInventoryItem::UpdateVisual(APickUpItem *Item)
      {
      	if (UGridSlot *GridSlot = Cast<UGridSlot>(this->Slot))
      	{
      		ItemImage->SetRenderTransformAngle(Item->GetItemRotation());
      		if (Item->GetItemRotation() == 90.0f)
      		{
      			ItemImage->SetRenderTranslation(FVector2D(ItemGridSize * GridSlot->GetRowSpan(), 0.0f));
      		}
      		else
      		{
      			ItemImage->SetRenderTranslation(FVector2D(0.0f, 0.0f));
      		}
      
      		TArray<FVector2D> ShapeContain = Item->GetShape(Item->GetItemRotation());
      
      		FVector2D Corner;
      
      		for (int32 i = 0; i < ShapeContain.Num(); i++)
      		{
      			VisibilityContainer[i]->SetRenderTranslation(FVector2D(ShapeContain[i].X * ItemGridSize, ShapeContain[i].Y * ItemGridSize));
      
      			if (ShapeContain[i].X >= Corner.X && ShapeContain[i].Y >= Corner.Y)
      			{
      				Corner = ShapeContain[i];
      			}
      		}
      		FVector2D CalculateVector(Corner.X * ItemGridSize, Corner.Y * ItemGridSize);
      		ItemQuantityText->SetRenderTranslation(FVector2D(CalculateVector.X + 40.0f, CalculateVector.Y + 60.0f));
      	}
      }
   </code>
  </pre>
</br>
</br>
</br>

Equipment System
-
Unreal Engine 자료구조 TArray를 활용한 창작 장비창 UI 입니다.
버튼 인터렉션과 스크롤 기능, 3D캐릭터 표시 등을 구현하였습니다.

예시 영상
-
https://github.com/user-attachments/assets/43cdbc05-c1af-4fad-b369-e822d5ce609b

클래스 구조도
-
![Image](https://github.com/user-attachments/assets/96db9cd1-e2cc-408b-b5c2-e5695f1042a6)

핵심 기능
-
- ### Equipment
  > - 무기를 장착할 수 있는 각 부위들을 버튼으로 만들어 클릭 시 무기 리스트UI( UEquipmentContents )가 표시되게 구현했습니다.
  > - 각 버튼을 캔버스 패널의 슬롯으로 배치하고 해당 버튼을 클릭했을 때, 캔버스 패널의 좌표 공간을 사용해서 무기 리스트 UI의 크기와 위치를 설정해주었습니다.
  > - 무기 리스트 UI는 버튼 클릭 시 해당 UI의 인스턴스를 생성해주었고, 무기 리스트 UI에서 InitializeEquipmentContents함수를 호출하여 해당 클래스가 필요한 정보를 얻어왔습니다.
  
  <pre>
    <code>
      void UEquipment::ButtonInteraction(UCanvasPanel *InCanvas, UBorder *InBorder)
      {
      	if (CanCheckCreateWidget(InCanvas))
      	{
      		if (EquipmentContentsWidget)
      		{
      			EquipmentContents = CreateWidget<UEquipmentContents>(GetOwningPlayer(), EquipmentContentsWidget);
      			if (EquipmentContents)
      			{
      				EquipmentContents->InitializeEquipmentContents(this);
      				EquipmentContents->OnAddWeaponWidget.AddUObject(this, &UEquipment::AddWeaponItem);
      				EquipmentContents->OnRemoveWeaponWidget.AddUObject(this, &UEquipment::RemoveWeaponItem);
      
      				if (auto SlotPanel = InCanvas->AddChildToCanvas(EquipmentContents))
      				{
      					if (auto ButtonSlot = Cast<UCanvasPanelSlot>(InBorder->Slot))
      					{
      						SlotPanel->SetSize(FVector2D(ItemListSize.X, ItemListSize.Y));
      						SlotPanel->SetPosition(FVector2D(ButtonSlot->GetPosition().X + ItemListPos.X, ButtonSlot->GetPosition().Y + ItemListPos.Y));
      					}
      				}
      			}
      		}
      	}
      }
    </code>
  </pre> 
  
- ### EquipmentContents
  > - InitializeEquipmentContents함수가 호출될 때, RefreshEquipmentSlot함수를 함께 호출하여 무기 리스트에서 필요한 정보들을 가져왔고, 리스트의 각 슬롯들을 만들어주었습니다.
  > - 무기 리스트에 표시되는 슬롯은 InventoryComponent에서 설정했던 InventorySize를 가져와서 해당 변수의 값 만큼 생성해주었고, TArray::IsValidIndex함수를 활용하여 배열의 Index가 유효한지 체크하여 Index의 값이 nullptr이 아니고 Index가 유효하다면 ItemList의 Index 정보를 무기 슬롯에 넘겨주었고, 그게 아니라면 무기 슬롯에 nullptr을 넘겨주어 빈 공간으로 표시되게 구현했습니다.
  
  <pre>
    <code>
      void UEquipmentContents::InitializeEquipmentContents(UEquipment* EquipmentWidget)
      {
                                            .
                                            .
                                            .
      	RefreshEquipmentSlot();
      	OnEquipWeaponWidget.AddUObject(this, &UEquipmentContents::EquipWeaponWidget);
      }
      
      void UEquipmentContents::InitEssentialData()
      {
      	Player = Cast<APlayerCharacter>(GetOwningPlayerPawn());
      	if (Player)
      	{
      		InventoryComponent = Player->GetItemInventory();
      		if (InventoryComponent)
      		{
      			ItemList = InventoryComponent->GetWeaponInventory();
      			ListSize = InventoryComponent->GetWeaponInventorySize();
      
      			InventoryComponent->OnInventoryUpdate.AddUObject(this, &UEquipmentContents::RefreshEquipmentSlot);
      		}
      	}
      }
      
      void UEquipmentContents::RefreshEquipmentSlot()
      {
      	InitEssentialData();
      	WidgetList.Empty();
      	ContentsBox->ClearChildren();
      	for (int32 i = 0; i < ListSize; i++)
      	{
      		if (EquipmentSlotWidget)
      		{
      			EquipmentSlot = CreateWidget<UEquipmentSlot>(GetOwningPlayer(), EquipmentSlotWidget);
      			if (EquipmentSlot)
      			{
      				EquipmentSlot->OnAddWidget.AddUObject(this, &UEquipmentContents::AddWeaponWidget);
      				EquipmentSlot->OnRemoveWidget.AddUObject(this, &UEquipmentContents::RemoveWeaponWidget);
      
      				if (ItemList.IsValidIndex(i))
      				{
      					if (ItemList[i])
      					{
      						EquipmentSlot->InitializeEquipmentSlot(this, ItemList[i]);
      
      						WidgetList.Add(EquipmentSlot);
      					}
      				}
      				else
      				{
      					EquipmentSlot->InitializeEquipmentSlot(this, nullptr);
      				}
      				ContentsBox->AddChild(EquipmentSlot);
      			}
      		}
      	}
      }
    </code>
  </pre> 
  
  - ### EquipmentSlot
  > - InitializeEquipmentSlot함수가 호출 될 때, SetIteminfo함수를 함께 호출하여 EquipmentContents에서 받아온 정보를 위젯 요소들에 적용시켜주었고, 받아온 값이 nullptr일 경우에는 투명도를 0으로 만들어 보이지 않게 해주었습니다.
  > - 해당 클래스는 버튼으로 만들지 않고, 마우스 상호작용과 관련된 Native함수들을 이용하여 버튼 처럼 보일 수 있게 구현했습니다.
  > - 화면에 위치한 EquipmentSlot을 클릭했을 때, 델리게이트를 이용하여 해당 클래스가 가지고 있는 ItemInfo를 상위 클래스로 다시 넘겨주는 방식으로 값을 이동시켜주었습니다.
  
  
  <pre>
    <code>
      void UEquipmentSlot::InitializeEquipmentSlot(UEquipmentContents *EquipContentsWidget, APickUpItem *WeaponItem)
      {
      	if (EquipContentsWidget)
      	{
      		EquipContents = EquipContentsWidget;
      		ItemInfo = WeaponItem;
      		if (EquipContents)
      		{
      			MainWeaponSlotState = EquipContents->GetWeaponButtonStateMain() && EquipContents->GetEquipSlotMain();
      			SubWeaponSlotState = EquipContents->GetWeaponButtonStateSub() && EquipContents->GetEquipSlotSub();
      			RangedWeaponSlotState = EquipContents->GetWeaponButtonStateRanged() && EquipContents->GetEquipSlotRanged();
      		}
      		WeaponImage->SetRenderOpacity(0.0f);
      		HoverImage->SetRenderOpacity(0.0f);
      
      		SetItemInfo(ItemInfo);
      	}
      }
      void UEquipmentSlot::SetItemInfo(APickUpItem *Item)
      {
      	if (Item)
      	{
      		WeaponImage->SetRenderOpacity(1.0f);
      		WeaponImage->SetBrushFromTexture(Item->GetItemTexture());
      	}
      	else
      	{
      		WeaponImage->SetRenderOpacity(0.0f);
      	}
      }

      FReply UEquipmentSlot::NativeOnMouseButtonDown(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent)
      {
      	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
      	{
      		if (ItemInfo && EquipContents)
      		{
      			if (MainWeaponSlotState)
      			{
      				OnAddWidget.Broadcast(EquipContents->GetEquipSlotMain());
      			}
      			if (SubWeaponSlotState)
      			{
      				OnAddWidget.Broadcast(EquipContents->GetEquipSlotSub());
      			}
      			if (RangedWeaponSlotState)
      			{
      				OnAddWidget.Broadcast(EquipContents->GetEquipSlotRanged());
      			}
      			EquipContents->OnEquipWeaponWidget.Broadcast(this);
      			OnRemoveWidget.Broadcast(this);
      			return FReply::Handled();
      		}
      	}
      	return FReply::Unhandled();
      }
    </code>
  </pre> 

  - ### Equipment
  > - EquipmentSlot에서 보낸 정보를 FSlateBrush 구조체를 사용하여 Button의 각 상태별 이미지를 바꿔주었습니다.
  
  <pre>
    <code>
      void UEquipment::EquipWeaponItem(UEquipmentSlot *SlotWidget)
      {
      	if (bIsActiveMainWeapon_BTN && SlotWidget)
      	{
      		MainWeaponSlot = SlotWidget;
      		SetButtonStyle(MainWeaponSlot, MainWeaponButton);
      		EquipMainWeapon();
      	}
      	if (bIsActiveSubWeapon_BTN && SlotWidget)
      	{
      		SubWeaponSlot = SlotWidget;
      		SetButtonStyle(SubWeaponSlot, SubWeaponButton);
      		EquipSubWeapon();
      	}
      	if (bIsActiveRangedWeapon_BTN && SlotWidget)
      	{
      		RangedWeaponSlot = SlotWidget;
      		SetButtonStyle(RangedWeaponSlot, RangedWeaponButton);
      		EquipRangedWeapon();
      	}
      }
      
      void UEquipment::SetButtonStyle(UEquipmentSlot *SlotWidget, UButton *ToButton)
      {
      	if (ToButton)
      	{
      		FSlateBrush NormalBrush;
      		NormalBrush.SetResourceObject(SlotWidget->GetWeaponItem()->GetItemTexture());
      		NormalBrush.TintColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f); 
      
      		FSlateBrush HoverBrush;
      		HoverBrush.SetResourceObject(SlotWidget->GetWeaponItem()->GetItemTexture());
      		HoverBrush.TintColor = FLinearColor(0.4f, 0.4f, 0.4f, 1.0f);
      
      		FSlateBrush PressedBrush;
      		PressedBrush.SetResourceObject(SlotWidget->GetWeaponItem()->GetItemTexture());
      		PressedBrush.TintColor = FLinearColor(0.1f, 0.1f, 0.1f, 1.0f);
      
      		FButtonStyle ButtonStyle;
      		ButtonStyle.Normal = NormalBrush;
      		ButtonStyle.Hovered = HoverBrush;
      		ButtonStyle.Pressed = PressedBrush;
      		ToButton->SetStyle(ButtonStyle);
      	}
      }
    </code>
  </pre> 

Item Pool
-
![Image](https://github.com/user-attachments/assets/77f2f276-c5e1-4c6d-be38-1d8229c3a76f)
- ### Item Pool Component
  > - TMap을 활용해서 EditList와 ItemPool을 만들어 주었습니다.
  > - EditList는 게임 디자이너(게임 기획자)가 레벨에 배치할 Item의 데이터와 개수를 정할 수 있도록 FName과 int32를 Key와 Value로 선언했고, 같은 타입의 Key와 실제 Spawn될 Item의 인스턴스들을 담을 배열 정보를 구조체로 만들어 Value로 가지고 있는 ItemPool을 선언해주었습니다.
  
 <pre>
    <code>
      struct FItemPoolStruct
      {
      	GENERATED_USTRUCT_BODY()
      
      	TArray<APickUpItem *> ItemList;
      	FItemPoolStruct()
      	{
      		ItemList.Empty();
      	}
      	void AddToItemList(APickUpItem* Item)
      	{
      		ItemList.Add(Item);
      	}
      	void RemoveToItemList()
      	{
      		ItemList.Pop();
      	}
      	bool ItemListIsEmpty()
      	{
      		return ItemList.IsEmpty();
      	}
      };
          
      class UE5_TIMETRAVLEHUNTER_API UItemPoolComponent : public UActorComponent
      {
      public:	
      	UPROPERTY(EditAnywhere, Category = "Class | BasicProperty")
      	TMap<FName, int32> EditList;
      
      private:
      	UPROPERTY(EditAnywhere, Category = "Item Data", meta = (AllowPrivateAccess = "true"))
      	UDataTable *ItemDataTable;
      
      	UPROPERTY(VisibleAnywhere, Category = "PoolMap")
      	TMap<FName, FItemPoolStruct> ItemPool;
      };
          
    </code>
  </pre> 
  
> - 다음으로 UDataTable::GetRowMap함수를 활용하여 DataTable의 Row들을 Map으로 받아왔고, Item의 인스턴스가 생성될 때 해당 Map을 순회하며 각 Row에 담겨져 있는 정보를 Item에 넣어주었습니다.
> - GetRowMap으로 DataTable을 받아올 경우 Value가 uint8타입으로 넘어오는 바람에 언리얼 엔진의 Cast를 사용할 수 없었습니다. 그래서 어쩔 수 없이 reinterpret_cast를 사용하여 강제로 형변환 해서 사용했습니다.
> - Item들은 AActor::SpawnActor를 사용하여 인스턴스화 해주었고, Item의 인스턴스를 배열에 담기전에 Visibility와 Collision을 모두 비활성화 시켜 게임에서는 보이지 않게 만들었습니다.

  <pre>
    <code>
      void UItemPoolComponent::InitializePool()
      {
      	for (const auto &e : EditList)
      	{
      		ItemPool.Add(e.Key);
      	}
      	TArray<FName> SubArray;
      	EditList.GetKeys(SubArray);
      	TMap<FName, uint8 *> RowMap = ItemDataTable->GetRowMap();
      	TArray<FName> MapArray;
      	RowMap.GetKeys(MapArray);
      	for (int32 i = 0; i < SubArray.Num(); i++)
      	{
      		FItemPoolStruct ItemPoolStruct;
      		for (int32 j = 0; j < EditList[SubArray[i]]; j++)
      		{
      			PickUpItem = GetWorld()->SpawnActor<APickUpItem>(PickUpItemClass);
      			PickUpItem->SetActivateItem(false);
      			ItemPoolStruct.AddToItemList(PickUpItem);
      		}
      		if (RowMap.Contains(SubArray[i])) 
      		{
      			if (auto RowData = reinterpret_cast<FItemData *>(RowMap[SubArray[i]]))
      			{
      				for (APickUpItem *Item : ItemPoolStruct.ItemList)
      				{
      					Item->SetItemData(SubArray[i], RowData);
      				}
      			}
      		}
      		ItemPool[SubArray[i]] = ItemPoolStruct;
      	}
      	bIsFillItemPool = true;
      }
    </code>
  </pre> 

  > - 레벨에 Item이 Spawn될 때에는 UseItemOfPool 함수를 호출하여 Item의 인스턴스를 담고 있는 배열의 마지막 Index부터 빼내어 SpawnPoint로 넘겨주었습니다.
  > - Item의 인스턴스를 넘겨줄 때에는 기존에 꺼두었던 Visibility와 Collision을 다시 활성화 시켜 게임에서 보일 수 있게 해주었습니다.

  <pre>
    <code>
         ========= ItemPoolComponent.cpp =========
      
      APickUpItem *UItemPoolComponent::UseItemOfPool(FName ItemRowName)
      {
      	TArray<FName> RowArray;
      	ItemPool.GetKeys(RowArray);
      	APickUpItem *PickItem{};
      	for (int32 i = 0; i < RowArray.Num(); i++)
      	{
      		if (RowArray[i] == ItemRowName)
      		{
      			if (!ItemPool[RowArray[i]].ItemListIsEmpty())
      			{
      				PickItem = ItemPool[RowArray[i]].ItemList[ItemPool[RowArray[i]].ItemList.Num() - 1];
      				PickItem->SetActivateItem(true);
      				ItemPool[RowArray[i]].RemoveToItemList();
      			}
      		}
      	}
      	return PickItem;
      }
          
          ========= SpawnPoint.cpp =========
          
      void ASpawnPoint::SpawnObject()
      {
      	if (ItemSpawnType == ESpawnType::PickUpItem)
      	{
      		SpawnItem = ItemPoolComponent->UseItemOfPool(ItemRowName);
      		if (SpawnItem)
      		{
      			SpawnItem->SetActorLocation(GetActorLocation());
      			SpawnItem->SetCurrentQuantity(ItemQuantity);
      			this->Destroy();
      		}
      	}
      }
    </code>
  </pre> 

> - SpawnPoint에서 Item을 Spawn할 때에는 Pool에 있는 개수보다 SpawnPoint가 많아지는 경우를 고려하여 CheckItemPoolIsEmpty함수를 통해 Item을 담고 있는 배열이 비어있는게 아니라면 Spawn할 수 있도록 구현했습니다.

  <pre>
    <code>
      
      bool UItemPoolComponent::CheckItemPoolIsEmpty()
      {
      	bool bIsItemPoolEmpty{ false };
      	TArray<FName> RowArray;
      	ItemPool.GetKeys(RowArray);
      	for (int32 i = 0; i < RowArray.Num(); i++)
      	{
      		if (ItemPool[RowArray[i]].ItemListIsEmpty())
      		{
      			bIsItemPoolEmpty = true;
      			bIsFillItemPool = false;
      		}
      		else
      		{
      			bIsItemPoolEmpty = false;
      			bIsFillItemPool = true;
      		}
      	}
      	return bIsItemPoolEmpty;
      }

    </code>
  </pre> 

> - 마지막으로 Player와 아이템이 상호작용하면 다시 Item의 Visibility와 Collision을 비활성화 시켜 게임에서 사라진 것 처럼 보일 수 있게 해주었고, ReturnItemToPool 함수를 사용하여 다시 ItemPool에 집어넣어 재사용할 수 있게 해주었습니다.

  <pre>
    <code>
         ========= ItemPoolComponent.cpp =========
      
      void UItemPoolComponent::ReturnItemToPool(APickUpItem *Item)
      {
      	if (Item)
      	{
      		TArray<FName> RowArray;
      		ItemPool.GetKeys(RowArray);
      		for (int32 i = 0; i < RowArray.Num(); i++)
      		{
      			if (RowArray[i] == Item->GetItemRowName())
      			{
      				Item->SetActivateItem(false);
      				ItemPool[RowArray[i]].AddToItemList(Item);
      			}
      		}
      	}
      }
    </code>
  </pre> 
